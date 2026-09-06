/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

/**
 * @file kernel_fdt.cpp
 * @brief KernelFdt 实现
 *
 * 对应接口：src/include/kernel_fdt.hpp
 * 存放路径：src/kernel_fdt.cpp
 */

#include "kernel_fdt.hpp"

#include <libfdt_env.h>
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
#include <libfdt.h>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include <array>
#include <cstring>

#include "kernel_log.hpp"
#include "nk_cassert"

namespace {
/// 从 FDT 属性缓冲区安全读取大端 u64/u32（memcpy 避免非对齐读的 UB）
auto LoadBeU64(const void* data) -> uint64_t {
  uint64_t value = 0;
  std::memcpy(&value, data, sizeof(value));
  return fdt64_to_cpu(value);
}
auto LoadBeU32(const void* data) -> uint32_t {
  uint32_t value = 0;
  std::memcpy(&value, data, sizeof(value));
  return fdt32_to_cpu(value);
}
}  // namespace

// ─────────────────────────────────────────────────────────────────
// 构造函数
// ─────────────────────────────────────────────────────────────────

KernelFdt::KernelFdt(uint64_t header)
    : fdt_header_(reinterpret_cast<fdt_header*>(header)) {
  ValidateFdtHeader().or_else([](Error err) -> Expected<void> {
    klog::Err("KernelFdt init failed: %s\n", err.message());
    while (true) {
      cpu_io::Pause();
    }
    return {};
  });

  klog::Debug(
      "Load dtb at [0x%llX], size [0x%X]\n",
      static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(fdt_header_)),
      static_cast<unsigned>(fdt32_to_cpu(fdt_header_->totalsize)));
}

// ─────────────────────────────────────────────────────────────────
// GetCoreCount
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::GetCoreCount() const -> Expected<size_t> {
  nk_assert_msg(fdt_header_ != nullptr, "fdt_header_ is null");

  return CountNodesByDeviceType("cpu").and_then(
      [](size_t count) -> Expected<size_t> {
        if (count == 0) {
          return std::unexpected(Error(ErrorCode::kFdtNodeNotFound));
        }
        return count;
      });
}

// ─────────────────────────────────────────────────────────────────
// CheckPSCI
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::CheckPSCI() const -> Expected<void> {
  nk_assert_msg(fdt_header_ != nullptr, "fdt_header_ is null");

  return FindNode("/psci").and_then([this](int offset) -> Expected<void> {
    return GetPsciMethod(offset).and_then(
        [this, offset](const char* method) -> Expected<void> {
          klog::Debug("PSCI method: %s\n", method);
          if (strcmp(method, "smc") != 0) {
            return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
          }
          return ValidatePsciFunctionIds(offset);
        });
  });
}

// ─────────────────────────────────────────────────────────────────
// GetMemory
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::GetMemory() const -> Expected<std::pair<uint64_t, size_t>> {
  nk_assert_msg(fdt_header_ != nullptr, "fdt_header_ is null");

  return FindNode("/memory").and_then(
      [this](int offset) -> Expected<std::pair<uint64_t, size_t>> {
        return GetRegProperty(offset).transform(
            [](std::pair<uint64_t, size_t> reg) { return reg; });
      });
}

// ─────────────────────────────────────────────────────────────────
// GetSerial
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::GetSerial() const
    -> Expected<std::tuple<uint64_t, size_t, uint32_t>> {
  nk_assert_msg(fdt_header_ != nullptr, "fdt_header_ is null");

  auto chosen_offset = FindNode("/chosen");
  if (!chosen_offset.has_value()) {
    return std::unexpected(chosen_offset.error());
  }

  int len = 0;
  const auto* prop =
      fdt_get_property(fdt_header_, chosen_offset.value(), "stdout-path", &len);
  if (prop == nullptr || len <= 0) {
    return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
  }

  const char* stdout_path = reinterpret_cast<const char*>(prop->data);
  // stdout-path 在 DTB 中是 NUL 结尾字符串，但 len 只约束属性字节数：
  // 拷贝封顶到 buffer-1 并强制补 NUL，避免 strchr 读到越界。
  std::array<char, 256> path_buffer;
  size_t copy_len = static_cast<size_t>(len);
  if (copy_len >= path_buffer.max_size()) {
    copy_len = path_buffer.max_size() - 1;
  }
  std::memcpy(path_buffer.data(), stdout_path, copy_len);
  path_buffer[copy_len] = '\0';

  // 去掉路径中的波特率后缀（如 "serial0:115200n8" → "serial0"）
  char* colon = strchr(path_buffer.data(), ':');
  if (colon != nullptr) {
    *colon = '\0';
  }

  // 解析别名（如 "&serial0" → 实际路径）
  int stdout_offset = -1;
  if (path_buffer[0] == '&') {
    const char* alias = path_buffer.data() + 1;
    const char* aliased_path = fdt_get_alias(fdt_header_, alias);
    if (aliased_path != nullptr) {
      stdout_offset = fdt_path_offset(fdt_header_, aliased_path);
    }
  } else {
    stdout_offset = fdt_path_offset(fdt_header_, path_buffer.data());
  }

  if (stdout_offset < 0) {
    return std::unexpected(Error(ErrorCode::kFdtNodeNotFound));
  }

  // 解析 reg 属性（基地址 + 大小）
  prop = fdt_get_property(fdt_header_, stdout_offset, "reg", &len);
  if (prop == nullptr) {
    return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
  }

  // TODO(接口重构)：reg 应按父节点 #address-cells/#size-cells 解析；
  // 目前固定按「多段 <addr64, size64>」取最后一对（与 GetRegProperty 一致）。
  uint64_t base = 0;
  size_t size = 0;
  const auto* data8 = reinterpret_cast<const uint8_t*>(prop->data);
  constexpr int kPairBytes = 2 * static_cast<int>(sizeof(uint64_t));
  for (int off = 0; off + kPairBytes <= len; off += kPairBytes) {
    base = LoadBeU64(data8 + off);
    size = static_cast<size_t>(LoadBeU64(data8 + off + sizeof(uint64_t)));
  }

  // 解析 interrupts 属性（中断号）
  prop = fdt_get_property(fdt_header_, stdout_offset, "interrupts", &len);
  if (prop == nullptr) {
    return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
  }

  uint32_t irq = 0;
  if (len >= static_cast<int>(sizeof(uint32_t))) {
    irq = LoadBeU32(prop->data);
  }

  return std::tuple{base, size, irq};
}

// ─────────────────────────────────────────────────────────────────
// GetTimebaseFrequency
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::GetTimebaseFrequency() const -> Expected<uint32_t> {
  nk_assert_msg(fdt_header_ != nullptr, "fdt_header_ is null");

  return FindNode("/cpus").and_then([this](int offset) -> Expected<uint32_t> {
    int len = 0;
    const auto* prop = reinterpret_cast<const uint32_t*>(
        fdt_getprop(fdt_header_, offset, "timebase-frequency", &len));
    if (prop == nullptr) {
      return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
    }
    if (len != sizeof(uint32_t)) {
      return std::unexpected(Error(ErrorCode::kFdtInvalidPropertySize));
    }
    return fdt32_to_cpu(*prop);
  });
}

// ─────────────────────────────────────────────────────────────────
// GetGIC
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::GetGIC() const
    -> Expected<std::tuple<uint64_t, size_t, uint64_t, size_t>> {
  nk_assert_msg(fdt_header_ != nullptr, "fdt_header_ is null");

  auto offset = FindCompatibleNode("arm,gic-v3");
  if (!offset.has_value()) {
    return std::unexpected(offset.error());
  }

  int len = 0;
  const auto* prop = fdt_get_property(fdt_header_, offset.value(), "reg", &len);
  if (prop == nullptr) {
    return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
  }

  uint64_t dist_base = 0;
  size_t dist_size = 0;
  uint64_t redist_base = 0;
  size_t redist_size = 0;

  // TODO(接口重构)：reg 的 cell 含义由父节点 #address-cells/#size-cells
  // 决定；此处按 GICv3 常见布局（<dist 2cell><redist 2cell>）读取。
  const auto* data8 = reinterpret_cast<const uint8_t*>(prop->data);
  if (len >= 2 * static_cast<int>(sizeof(uint64_t))) {
    dist_base = LoadBeU64(data8);
    dist_size = static_cast<size_t>(LoadBeU64(data8 + sizeof(uint64_t)));
  }
  if (len >= 4 * static_cast<int>(sizeof(uint64_t))) {
    redist_base = LoadBeU64(data8 + 2 * sizeof(uint64_t));
    redist_size = static_cast<size_t>(LoadBeU64(data8 + 3 * sizeof(uint64_t)));
  }

  return std::tuple{dist_base, dist_size, redist_base, redist_size};
}

// ─────────────────────────────────────────────────────────────────
// GetGicDist / GetGicCpu
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::GetGicDist() const -> Expected<std::pair<uint64_t, size_t>> {
  return GetGIC().transform(
      [](std::tuple<uint64_t, size_t, uint64_t, size_t> gic) {
        return std::pair{std::get<0>(gic), std::get<1>(gic)};
      });
}

auto KernelFdt::GetGicCpu() const -> Expected<std::pair<uint64_t, size_t>> {
  return GetGIC().transform(
      [](std::tuple<uint64_t, size_t, uint64_t, size_t> gic) {
        return std::pair{std::get<2>(gic), std::get<3>(gic)};
      });
}

// ─────────────────────────────────────────────────────────────────
// GetAarch64Intid
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::GetAarch64Intid(const char* compatible) const
    -> Expected<uint64_t> {
  nk_assert_msg(fdt_header_ != nullptr, "fdt_header_ is null");

  auto offset = FindEnabledCompatibleNode(compatible);
  if (!offset.has_value()) {
    return std::unexpected(offset.error());
  }

  int len = 0;
  const auto* prop =
      fdt_get_property(fdt_header_, offset.value(), "interrupts", &len);
  if (prop == nullptr) {
    return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
  }

  const auto* data8 = reinterpret_cast<const uint8_t*>(prop->data);

#ifdef NAILONG_KERNEL_DEBUG
  // interrupts 三元组（type/intid/trigger+cpuid），按 len 字节数安全遍历
  constexpr int kTripletBytes = 3 * static_cast<int>(sizeof(uint32_t));
  for (int off = 0; off + kTripletBytes <= len; off += kTripletBytes) {
    auto type = LoadBeU32(data8 + off);
    auto intid = LoadBeU32(data8 + off + sizeof(uint32_t));
    auto trigger = LoadBeU32(data8 + off + 2 * sizeof(uint32_t)) & 0xF;
    auto cpuid_mask = LoadBeU32(data8 + off + 2 * sizeof(uint32_t)) & 0xFF00;
    klog::Debug("type: %d, intid: %d, trigger: %d, cpuid_mask: %d\n", type,
                intid, trigger, cpuid_mask);
  }
#endif

  // 不同 compatible 对应的 cell 下标不同，先按 len 校验再读，避免越界
  uint64_t intid = 0;
  int cell_index = -1;
  if (strcmp(compatible, "arm,armv8-timer") == 0) {
    cell_index = 7;
  } else if (strcmp(compatible, "arm,pl011") == 0) {
    cell_index = 1;
  }
  if (cell_index >= 0 &&
      len >= (cell_index + 1) * static_cast<int>(sizeof(uint32_t))) {
    intid = LoadBeU32(data8 + cell_index * static_cast<int>(sizeof(uint32_t)));
  }

  return intid;
}

// ─────────────────────────────────────────────────────────────────
// GetPlic
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::GetPlic() const
    -> Expected<std::tuple<uint64_t, uint64_t, uint32_t, uint32_t>> {
  nk_assert_msg(fdt_header_ != nullptr, "fdt_header_ is null");

  auto offset = FindCompatibleNode("sifive,plic-1.0.0");
  if (!offset.has_value()) {
    offset = FindCompatibleNode("riscv,plic0");
  }
  if (!offset.has_value()) {
    return std::unexpected(offset.error());
  }

  int len = 0;

  // interrupts-extended 格式：<cpu_phandle irq_id> 成对，每对代表一个上下文
  const auto* prop = fdt_get_property(fdt_header_, offset.value(),
                                      "interrupts-extended", &len);
  if (prop == nullptr) {
    return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
  }
  uint32_t context_count = (len / sizeof(uint32_t)) / 2;

  // ndev：PLIC 支持的最大中断源数量（u32）
  prop = fdt_get_property(fdt_header_, offset.value(), "riscv,ndev", &len);
  if (prop == nullptr) {
    return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
  }
  if (len < static_cast<int>(sizeof(uint32_t))) {
    return std::unexpected(Error(ErrorCode::kFdtInvalidPropertySize));
  }
  uint32_t ndev = LoadBeU32(prop->data);

  auto reg = GetRegProperty(offset.value());
  if (!reg.has_value()) {
    return std::unexpected(reg.error());
  }

  return std::tuple{reg.value().first, reg.value().second, ndev, context_count};
}

// ─────────────────────────────────────────────────────────────────
// private: ValidateFdtHeader
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::ValidateFdtHeader() const -> Expected<void> {
  nk_assert_msg(fdt_header_ != nullptr, "fdt_header_ is null");
  if (fdt_check_header(fdt_header_) != 0) {
    return std::unexpected(Error(ErrorCode::kFdtInvalidHeader));
  }
  return {};
}

// ─────────────────────────────────────────────────────────────────
// private: FindNode
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::FindNode(const char* path) const -> Expected<int> {
  auto offset = fdt_path_offset(fdt_header_, path);
  if (offset < 0) {
    return std::unexpected(Error(ErrorCode::kFdtNodeNotFound));
  }
  return offset;
}

// ─────────────────────────────────────────────────────────────────
// private: FindCompatibleNode
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::FindCompatibleNode(const char* compatible) const
    -> Expected<int> {
  auto offset = fdt_node_offset_by_compatible(fdt_header_, -1, compatible);
  if (offset < 0) {
    return std::unexpected(Error(ErrorCode::kFdtNodeNotFound));
  }
  return offset;
}

// ─────────────────────────────────────────────────────────────────
// private: FindEnabledCompatibleNode
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::FindEnabledCompatibleNode(const char* compatible) const
    -> Expected<int> {
  int offset = -1;
  while (true) {
    offset = fdt_node_offset_by_compatible(fdt_header_, offset, compatible);
    if (offset < 0) {
      return std::unexpected(Error(ErrorCode::kFdtNodeNotFound));
    }

    int len = 0;
    const auto* status_prop =
        fdt_get_property(fdt_header_, offset, "status", &len);

    // 没有 status 属性默认为 okay
    if (status_prop == nullptr) {
      return offset;
    }

    const char* status = reinterpret_cast<const char*>(status_prop->data);
    if (strcmp(status, "okay") == 0 || strcmp(status, "ok") == 0) {
      return offset;
    }
    // status="disabled"，继续找下一个
  }
}

// ─────────────────────────────────────────────────────────────────
// private: GetRegProperty
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::GetRegProperty(int offset) const
    -> Expected<std::pair<uint64_t, size_t>> {
  int len = 0;
  const auto* prop = fdt_get_property(fdt_header_, offset, "reg", &len);
  if (prop == nullptr) {
    return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
  }

  // TODO(接口重构)：reg 应按父节点 #address-cells/#size-cells 解析；
  // 目前固定按「多段 <addr64, size64>」取最后一对（与 GetSerial 一致）。
  uint64_t base = 0;
  size_t size = 0;
  const auto* data8 = reinterpret_cast<const uint8_t*>(prop->data);
  constexpr int kPairBytes = 2 * static_cast<int>(sizeof(uint64_t));
  for (int off = 0; off + kPairBytes <= len; off += kPairBytes) {
    base = LoadBeU64(data8 + off);
    size = static_cast<size_t>(LoadBeU64(data8 + off + sizeof(uint64_t)));
  }
  return std::pair{base, size};
}

// ─────────────────────────────────────────────────────────────────
// private: CountNodesByDeviceType
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::CountNodesByDeviceType(const char* device_type) const
    -> Expected<size_t> {
  size_t count = 0;
  int offset = -1;

  while (true) {
    offset = fdt_next_node(fdt_header_, offset, nullptr);
    if (offset < 0) {
      if (offset != -FDT_ERR_NOTFOUND) {
        return std::unexpected(Error(ErrorCode::kFdtParseFailed));
      }
      break;
    }

    const auto* prop =
        fdt_get_property(fdt_header_, offset, "device_type", nullptr);
    if (prop != nullptr) {
      const char* type = reinterpret_cast<const char*>(prop->data);
      if (strcmp(type, device_type) == 0) {
        ++count;
      }
    }
  }

  return count;
}

// ─────────────────────────────────────────────────────────────────
// private: GetPsciMethod
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::GetPsciMethod(int offset) const -> Expected<const char*> {
  int len = 0;
  const auto* prop = fdt_get_property(fdt_header_, offset, "method", &len);
  if (prop == nullptr) {
    return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
  }
  return reinterpret_cast<const char*>(prop->data);
}

// ─────────────────────────────────────────────────────────────────
// private: ValidatePsciFunctionIds
// ─────────────────────────────────────────────────────────────────

auto KernelFdt::ValidatePsciFunctionIds(int offset) const -> Expected<void> {
  auto validate_id = [this, offset](const char* name,
                                    uint64_t expected) -> Expected<void> {
    int len = 0;
    const auto* prop = fdt_get_property(fdt_header_, offset, name, &len);
    if (prop != nullptr && static_cast<size_t>(len) >= sizeof(uint32_t)) {
      uint32_t id =
          fdt32_to_cpu(*reinterpret_cast<const uint32_t*>(prop->data));
      klog::Debug("PSCI %s function ID: 0x%X\n", name, id);
      if (id != expected) {
        klog::Err("PSCI %s function ID mismatch: expected 0x%X, got 0x%X\n",
                  name, expected, id);
        return std::unexpected(Error(ErrorCode::kFdtPropertyNotFound));
      }
    }
    return {};
  };

  return validate_id("cpu_on", kPsciCpuOnFuncId)
      .and_then([&]() { return validate_id("cpu_off", kPsciCpuOffFuncId); })
      .and_then(
          [&]() { return validate_id("cpu_suspend", kPsciCpuSuspendFuncId); });
}
