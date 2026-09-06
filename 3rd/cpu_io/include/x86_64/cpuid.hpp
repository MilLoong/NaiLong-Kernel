/**
 * @copyright Copyright The cpu_io Contributors
 * @brief x86_64 CPUID 指令操作
 */

#ifndef CPU_IO_INCLUDE_X86_64_CPUID_HPP_
#define CPU_IO_INCLUDE_X86_64_CPUID_HPP_

#include <sys/cdefs.h>

#include <array>
#include <cstdint>
#include <cstring>

namespace cpu_io {
namespace cpuid {

/// 实现细节命名空间
namespace detail {

/// CPUID 功能号常量
namespace leaf {
/// 基本功能号范围
/// 获取最大基本功能号和厂商字符串
static constexpr uint32_t kBasicInfo = 0x00000000;
/// 版本信息和特性位
static constexpr uint32_t kVersionInfo = 0x00000001;
/// 缓存和 TLB 描述符
static constexpr uint32_t kCacheInfo = 0x00000002;
/// 处理器序列号
static constexpr uint32_t kSerialNumber = 0x00000003;
/// 确定性缓存参数
static constexpr uint32_t kCacheParams = 0x00000004;
/// MONITOR/MWAIT 功能
static constexpr uint32_t kMonitorMwait = 0x00000005;
/// 热管理和功耗管理
static constexpr uint32_t kThermalPower = 0x00000006;
/// 扩展特性标志
static constexpr uint32_t kExtendedFeatures = 0x00000007;
/// 保留
static constexpr uint32_t kReserved1 = 0x00000008;
/// 直接缓存访问参数
static constexpr uint32_t kDirectCacheAccess = 0x00000009;
/// 架构性能监控
static constexpr uint32_t kArchPerfMon = 0x0000000A;
/// 扩展拓扑枚举
static constexpr uint32_t kExtendedTopology = 0x0000000B;

/// 扩展功能号范围
/// 获取最大扩展功能号
static constexpr uint32_t kExtendedInfo = 0x80000000;
/// 扩展版本信息和特性位
static constexpr uint32_t kExtendedVersionInfo = 0x80000001;
/// 品牌字符串部分1
static constexpr uint32_t kBrandString1 = 0x80000002;
/// 品牌字符串部分2
static constexpr uint32_t kBrandString2 = 0x80000003;
/// 品牌字符串部分3
static constexpr uint32_t kBrandString3 = 0x80000004;
/// 虚拟和物理地址大小
static constexpr uint32_t kAddressSize = 0x80000008;
}  // namespace leaf

/// CPUID 特性位定义
namespace feature {
/// CPUID.01H:EDX 特性位
namespace edx {
/// x87 FPU
static constexpr uint32_t kFpu = 1U << 0;
/// 虚拟 8086 模式扩展
static constexpr uint32_t kVme = 1U << 1;
/// 调试扩展
static constexpr uint32_t kDe = 1U << 2;
/// 页大小扩展
static constexpr uint32_t kPse = 1U << 3;
/// 时间戳计数器
static constexpr uint32_t kTsc = 1U << 4;
/// MSR 支持
static constexpr uint32_t kMsr = 1U << 5;
/// 物理地址扩展
static constexpr uint32_t kPae = 1U << 6;
/// 机器检查异常
static constexpr uint32_t kMce = 1U << 7;
/// CMPXCHG8B 指令
static constexpr uint32_t kCx8 = 1U << 8;
/// APIC 支持
static constexpr uint32_t kApic = 1U << 9;
/// SYSENTER/SYSEXIT
static constexpr uint32_t kSep = 1U << 11;
/// 内存类型范围寄存器
static constexpr uint32_t kMtrr = 1U << 12;
/// 页全局使能
static constexpr uint32_t kPge = 1U << 13;
/// 机器检查架构
static constexpr uint32_t kMca = 1U << 14;
/// 条件移动指令
static constexpr uint32_t kCmov = 1U << 15;
/// 页属性表
static constexpr uint32_t kPat = 1U << 16;
/// 36 位页大小扩展
static constexpr uint32_t kPse36 = 1U << 17;
/// 处理器序列号
static constexpr uint32_t kPsn = 1U << 18;
/// CLFLUSH 指令
static constexpr uint32_t kClfsh = 1U << 19;
/// 调试存储
static constexpr uint32_t kDs = 1U << 21;
/// 热监控和软件控制时钟
static constexpr uint32_t kAcpi = 1U << 22;
/// MMX 技术
static constexpr uint32_t kMmx = 1U << 23;
/// FXSAVE/FXRSTOR 指令
static constexpr uint32_t kFxsr = 1U << 24;
/// SSE 指令
static constexpr uint32_t kSse = 1U << 25;
/// SSE2 指令
static constexpr uint32_t kSse2 = 1U << 26;
/// 自监听
static constexpr uint32_t kSs = 1U << 27;
/// 多线程
static constexpr uint32_t kHtt = 1U << 28;
/// 热监控
static constexpr uint32_t kTm = 1U << 29;
/// 挂起中断使能
static constexpr uint32_t kPbe = 1U << 31;
}  // namespace edx

/// CPUID.01H:ECX 特性位
namespace ecx {
/// SSE3 指令
static constexpr uint32_t kSse3 = 1U << 0;
/// PCLMULQDQ 指令
static constexpr uint32_t kPclmulqdq = 1U << 1;
/// 64 位调试存储
static constexpr uint32_t kDtes64 = 1U << 2;
/// MONITOR/MWAIT 指令
static constexpr uint32_t kMonitor = 1U << 3;
/// CPL 限定调试存储
static constexpr uint32_t kDsCpl = 1U << 4;
/// 虚拟机扩展
static constexpr uint32_t kVmx = 1U << 5;
/// 安全模式扩展
static constexpr uint32_t kSmx = 1U << 6;
/// 增强 Intel SpeedStep 技术
static constexpr uint32_t kEist = 1U << 7;
/// 热监控 2
static constexpr uint32_t kTm2 = 1U << 8;
/// 补充 SSE3 指令
static constexpr uint32_t kSsse3 = 1U << 9;
/// L1 上下文 ID
static constexpr uint32_t kCnxtId = 1U << 10;
/// Silicon 调试接口
static constexpr uint32_t kSdbg = 1U << 11;
/// 融合乘加指令
static constexpr uint32_t kFma = 1U << 12;
/// CMPXCHG16B 指令
static constexpr uint32_t kCx16 = 1U << 13;
/// xTPR 更新控制
static constexpr uint32_t kXtpr = 1U << 14;
/// 性能/调试能力 MSR
static constexpr uint32_t kPdcm = 1U << 15;
/// 进程上下文标识符
static constexpr uint32_t kPcid = 1U << 17;
/// 直接缓存访问
static constexpr uint32_t kDca = 1U << 18;
/// SSE4.1 指令
static constexpr uint32_t kSse41 = 1U << 19;
/// SSE4.2 指令
static constexpr uint32_t kSse42 = 1U << 20;
/// x2APIC 支持
static constexpr uint32_t kX2Apic = 1U << 21;
/// MOVBE 指令
static constexpr uint32_t kMovbe = 1U << 22;
/// POPCNT 指令
static constexpr uint32_t kPopcnt = 1U << 23;
/// TSC 截止时间
static constexpr uint32_t kTscDeadline = 1U << 24;
/// AES 指令
static constexpr uint32_t kAes = 1U << 25;
/// XSAVE/XRSTOR 指令
static constexpr uint32_t kXsave = 1U << 26;
/// OS 启用 XSAVE
static constexpr uint32_t kOsxsave = 1U << 27;
/// AVX 指令
static constexpr uint32_t kAvx = 1U << 28;
/// 16 位浮点转换指令
static constexpr uint32_t kF16C = 1U << 29;
/// RDRAND 指令
static constexpr uint32_t kRdrand = 1U << 30;
/// 运行在虚拟机管理程序下
static constexpr uint32_t kHypervisor = 1U << 31;
}  // namespace ecx
}  // namespace feature

/// CPUID 结果结构
struct CpuidResult {
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
};

/**
 * @brief 执行 CPUID 指令
 * @param leaf 功能号
 * @param subleaf 子功能号
 * @return CpuidResult CPUID 结果
 */
static __always_inline auto ExecuteCpuid(uint32_t leaf,
                                         uint32_t subleaf = 0) -> CpuidResult {
  CpuidResult result;

  __asm__ volatile("cpuid"
                   : "=a"(result.eax), "=b"(result.ebx), "=c"(result.ecx),
                     "=d"(result.edx)
                   : "a"(leaf), "c"(subleaf)
                   : "memory");

  return result;
}

/**
 * @brief 检查特性位是否设置
 * @param value 寄存器值
 * @param feature_bit 特性位掩码
 * @return bool 是否支持该特性
 */
static __always_inline auto HasFeature(uint32_t value,
                                       uint32_t feature_bit) -> bool {
  return (value & feature_bit) != 0;
}

/**
 * @brief 获取最大基本功能号
 * @return uint32_t 最大基本功能号
 */
static __always_inline auto GetMaxBasicLeaf() -> uint32_t {
  auto result = ExecuteCpuid(leaf::kBasicInfo);
  return result.eax;
}

/**
 * @brief 检查是否支持指定特性（基于 CPUID.01H:EDX）
 * @param feature_bit 特性位掩码
 * @return bool 是否支持该特性
 */
static __always_inline auto HasFeatureEdx(uint32_t feature_bit) -> bool {
  auto result = ExecuteCpuid(leaf::kVersionInfo);
  return HasFeature(result.edx, feature_bit);
}

/**
 * @brief 检查是否支持指定特性（基于 CPUID.01H:ECX）
 * @param feature_bit 特性位掩码
 * @return bool 是否支持该特性
 */
static __always_inline auto HasFeatureEcx(uint32_t feature_bit) -> bool {
  auto result = ExecuteCpuid(leaf::kVersionInfo);
  return HasFeature(result.ecx, feature_bit);
}

}  // namespace detail

/**
 * @brief 获取 CPU 厂商字符串
 * @return std::array<char, 13> 厂商字符串（包含 null 终止符）
 */
static __always_inline auto GetVendorString() -> std::array<char, 13> {
  auto result = detail::ExecuteCpuid(detail::leaf::kBasicInfo);
  std::array<char, 13> vendor{};

  std::memcpy(vendor.data(), &result.ebx, 4);
  std::memcpy(vendor.data() + 4, &result.edx, 4);
  std::memcpy(vendor.data() + 8, &result.ecx, 4);
  vendor[12] = '\0';

  return vendor;
}

/**
 * @brief 获取 CPU 品牌字符串
 * @return std::array<char, 49> 品牌字符串（包含 null 终止符）
 */
static __always_inline auto GetBrandString() -> std::array<char, 49> {
  std::array<char, 49> brand{};

  // 检查是否支持扩展功能
  auto ext_info = detail::ExecuteCpuid(detail::leaf::kExtendedInfo);
  if (ext_info.eax < detail::leaf::kBrandString3) {
    return brand;
  }

  // 获取品牌字符串的三个部分
  auto brand1 = detail::ExecuteCpuid(detail::leaf::kBrandString1);
  auto brand2 = detail::ExecuteCpuid(detail::leaf::kBrandString2);
  auto brand3 = detail::ExecuteCpuid(detail::leaf::kBrandString3);

  std::memcpy(brand.data(), &brand1, 16);
  std::memcpy(brand.data() + 16, &brand2, 16);
  std::memcpy(brand.data() + 32, &brand3, 16);
  brand[48] = '\0';

  return brand;
}

/**
 * @brief 检查是否支持 APIC
 * @return bool 是否支持 APIC
 */
static __always_inline auto HasApic() -> bool {
  return detail::HasFeatureEdx(detail::feature::edx::kApic);
}

/**
 * @brief 检查是否支持 x2APIC
 * @return bool 是否支持 x2APIC
 */
static __always_inline auto HasX2Apic() -> bool {
  return detail::HasFeatureEcx(detail::feature::ecx::kX2Apic);
}

/**
 * @brief 检查是否支持 TSC
 * @return bool 是否支持 TSC
 */
static __always_inline auto HasTsc() -> bool {
  return detail::HasFeatureEdx(detail::feature::edx::kTsc);
}

/**
 * @brief 检查是否支持 MSR
 * @return bool 是否支持 MSR
 */
static __always_inline auto HasMsr() -> bool {
  return detail::HasFeatureEdx(detail::feature::edx::kMsr);
}

/**
 * @brief 获取 APIC ID
 * @return uint32_t APIC ID
 */
static __always_inline auto GetApicId() -> uint32_t {
  auto result = detail::ExecuteCpuid(detail::leaf::kVersionInfo);
  return (result.ebx >> 24) & 0xFF;
}

/**
 * @brief 获取扩展 APIC ID（如果支持）
 * @return uint32_t 扩展 APIC ID
 */
static __always_inline auto GetExtendedApicId() -> uint32_t {
  if (detail::GetMaxBasicLeaf() >= detail::leaf::kExtendedTopology) {
    auto result = detail::ExecuteCpuid(detail::leaf::kExtendedTopology, 0);
    return result.edx;
  }
  return GetApicId();
}

/**
 * @brief 获取逻辑处理器数量
 * @return uint32_t 逻辑处理器数量
 */
static __always_inline auto GetLogicalProcessorCount() -> uint32_t {
  auto result = detail::ExecuteCpuid(detail::leaf::kVersionInfo);
  return (result.ebx >> 16) & 0xFF;
}

}  // namespace cpuid
}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_CPUID_HPP_
