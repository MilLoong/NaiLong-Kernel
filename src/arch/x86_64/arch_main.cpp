/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include <cpu_io.h>

#include <array>
#include <cstdint>
#include <cstring>

#include "apic.h"
#include "arch.h"
#include "basic_info.hpp"
#include "kernel_elf.hpp"
#include "kernel_log.hpp"
#include "per_cpu.hpp"
#include "singleton.hpp"
#include "sipi.h"
#include "nk_iostream"

namespace {

// QEMU fw_cfg interface (I/O ports) for obtaining basic platform info.
// This avoids relying on BIOS e820 calls and works in long mode on QEMU.
//
// - selector port: 0x510 (16-bit)
// - data port:     0x511 (8-bit)
// Data is returned big-endian.
constexpr uint16_t kFwCfgSelectorPort = 0x510;
constexpr uint16_t kFwCfgDataPort = 0x511;
// FW_CFG_RAM_SIZE：历史上为 32-bit；按 BE 逐字节读 4 字节即可
constexpr uint16_t kFwCfgRamSize = 0x0003;

[[nodiscard]] auto FwCfgReadU32(uint16_t item) -> uint32_t {
  cpu_io::Out<uint16_t>(kFwCfgSelectorPort, item);
  uint32_t value = 0;
  for (int i = 0; i < 4; ++i) {
    value = (value << 8) | cpu_io::In<uint8_t>(kFwCfgDataPort);
  }
  return value;
}

/// gdt 描述符表，顺序与 cpu_io::detail::register_info::GdtrInfo 中的定义一致
std::array<cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor,
           cpu_io::detail::register_info::GdtrInfo::kMaxCount>
    kSegmentDescriptors = {
        // 第一个全 0
        cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor(),
        // 内核代码段描述符
        cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor(
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::Type::
                kCodeExecuteRead,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::S::
                kCodeData,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::DPL::
                kRing0,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::P::
                kPresent,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::AVL::
                kNotAvailable,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::L::
                k64Bit),
        // 内核数据段描述符
        cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor(
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::Type::
                kDataReadWrite,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::S::
                kCodeData,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::DPL::
                kRing0,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::P::
                kPresent,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::AVL::
                kNotAvailable,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::L::
                k64Bit),
        // 用户代码段描述符
        cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor(
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::Type::
                kCodeExecuteRead,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::S::
                kCodeData,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::DPL::
                kRing3,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::P::
                kPresent,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::AVL::
                kNotAvailable,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::L::
                k64Bit),
        // 用户数据段描述符
        cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor(
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::Type::
                kDataReadWrite,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::S::
                kCodeData,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::DPL::
                kRing3,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::P::
                kPresent,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::AVL::
                kNotAvailable,
            cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor::L::
                k64Bit),
};

cpu_io::detail::register_info::GdtrInfo::Gdtr gdtr{
    .limit =
        (sizeof(cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor) *
         cpu_io::detail::register_info::GdtrInfo::kMaxCount) -
        1,
    .base = kSegmentDescriptors.data(),
};

/// 设置 GDT 和段寄存器
void SetupGdtAndSegmentRegisters() {
  // 设置 gdt
  cpu_io::Gdtr::Write(gdtr);

  // 加载内核数据段描述符
  cpu_io::Ds::Write(
      sizeof(cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor) *
      cpu_io::detail::register_info::GdtrInfo::kKernelDataIndex);
  cpu_io::Es::Write(
      sizeof(cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor) *
      cpu_io::detail::register_info::GdtrInfo::kKernelDataIndex);
  cpu_io::Fs::Write(
      sizeof(cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor) *
      cpu_io::detail::register_info::GdtrInfo::kKernelDataIndex);
  cpu_io::Gs::Write(
      sizeof(cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor) *
      cpu_io::detail::register_info::GdtrInfo::kKernelDataIndex);
  cpu_io::Ss::Write(
      sizeof(cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor) *
      cpu_io::detail::register_info::GdtrInfo::kKernelDataIndex);
  // 加载内核代码段描述符
  cpu_io::Cs::Write(
      sizeof(cpu_io::detail::register_info::GdtrInfo::SegmentDescriptor) *
      cpu_io::detail::register_info::GdtrInfo::kKernelCodeIndex);
}

}  // namespace

BasicInfo::BasicInfo(int, const char**) {
  // Physical memory range on QEMU.
  // Keep the first 1MiB for legacy/firmware areas and start our allocatable
  // memory above that.
  constexpr uint64_t kUsablePhysBase = 0x100000;
  constexpr uint64_t kFallbackRamSize = 1024ULL * 1024 * 1024;  // 与 -m 1024M 一致
  constexpr uint64_t kMaxSaneRamSize = 16ULL * 1024 * 1024 * 1024;

  uint64_t ram_size = FwCfgReadU32(kFwCfgRamSize);
  // 部分 QEMU 对 fw_cfg 返回 host endian；异常值回退到预设大小
  if (ram_size <= kUsablePhysBase || ram_size > kMaxSaneRamSize) {
    ram_size = kFallbackRamSize;
  }

  physical_memory_addr = kUsablePhysBase;
  physical_memory_size = static_cast<size_t>(ram_size - kUsablePhysBase);

  kernel_addr = reinterpret_cast<uint64_t>(__executable_start);
  kernel_size = reinterpret_cast<uint64_t>(end) -
                reinterpret_cast<uint64_t>(__executable_start);

  elf_addr = kernel_addr;

  fdt_addr = 0;

  core_count = cpu_io::cpuid::GetLogicalProcessorCount();
}

void ArchInit(int, const char**) {
  Singleton<BasicInfo>::GetInstance() = BasicInfo(0, nullptr);
  nk_std::cout << Singleton<BasicInfo>::GetInstance();

  // 解析内核 elf 信息
  Singleton<KernelElf>::GetInstance() =
      KernelElf(Singleton<BasicInfo>::GetInstance().elf_addr);

  // 设置 GDT 和段寄存器
  SetupGdtAndSegmentRegisters();

  // 初始化 APIC
  Singleton<Apic>::GetInstance() =
      Apic(Singleton<BasicInfo>::GetInstance().core_count);
  Singleton<Apic>::GetInstance().InitCurrentCpuLocalApic().or_else(
      [](Error err) -> Expected<void> {
        klog::Err("Failed to initialize APIC: %s\n", err.message());
        while (true) {
          cpu_io::Pause();
        }
        return std::unexpected(err);
      });

  klog::Info("Hello x86_64 ArchInit\n");
}

void ArchInitSMP(int, const char**) {
  // 设置 GDT 和段寄存器
  SetupGdtAndSegmentRegisters();

  Singleton<Apic>::GetInstance().InitCurrentCpuLocalApic().or_else(
      [](Error err) -> Expected<void> {
        klog::Err("Failed to initialize APIC for AP: %s\n", err.message());
        while (true) {
          cpu_io::Pause();
        }
        return std::unexpected(err);
      });
}

void WakeUpOtherCores() {
  // 填充 sipi_params 结构体
  auto target_sipi_params = reinterpret_cast<sipi_params_t*>(sipi_params);
  target_sipi_params->cr3 = cpu_io::Cr3::Read();

  Singleton<Apic>::GetInstance().StartupAllAps(
      reinterpret_cast<uint64_t>(ap_start16),
      reinterpret_cast<size_t>(ap_start64_end) -
          reinterpret_cast<size_t>(ap_start16),
      kDefaultAPBase);
}

void InitTaskContext(cpu_io::CalleeSavedContext* task_context,
                     void (*entry)(void*), void* arg, uint64_t stack_top) {
  // 清零 → 填入口/参数/栈 → 首次调度跳到 kernel_thread_entry
  std::memset(task_context, 0, sizeof(cpu_io::CalleeSavedContext));

  task_context->ReturnAddress() =
      reinterpret_cast<uint64_t>(kernel_thread_entry);
  task_context->EntryFunction() = reinterpret_cast<uint64_t>(entry);
  task_context->EntryArgument() = reinterpret_cast<uint64_t>(arg);
  task_context->StackPointer() = stack_top;
}

void InitTaskContext(cpu_io::CalleeSavedContext* task_context,
                     cpu_io::TrapContext* trap_context_ptr,
                     uint64_t stack_top) {
  // 清零 → 填 trap_return/TrapContext/栈 → 首次调度进入用户返回路径
  std::memset(task_context, 0, sizeof(cpu_io::CalleeSavedContext));

  task_context->ReturnAddress() =
      reinterpret_cast<uint64_t>(kernel_thread_entry);
  task_context->EntryFunction() = reinterpret_cast<uint64_t>(trap_return);
  task_context->EntryArgument() = reinterpret_cast<uint64_t>(trap_context_ptr);
  task_context->StackPointer() = stack_top;
}
