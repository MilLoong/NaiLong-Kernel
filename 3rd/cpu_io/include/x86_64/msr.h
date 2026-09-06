/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_X86_64_MSR_H_
#define CPU_IO_INCLUDE_X86_64_MSR_H_

#include <cstdint>

namespace cpu_io {
/**
 * @brief MSR（Model Specific Register）相关定义
 */
namespace msr {

/// MSR 寄存器地址常量定义
/// @see Intel SDM Vol. 4

/// 扩展功能使能寄存器
static constexpr uint32_t kIa32Efer = 0xC0000080;
/// SYSCALL target address
static constexpr uint32_t kIa32Star = 0xC0000081;
/// Long mode SYSCALL target
static constexpr uint32_t kIa32Lstar = 0xC0000082;
/// Compatibility mode SYSCALL target
static constexpr uint32_t kIa32Cstar = 0xC0000083;
/// EFLAGS mask for syscall
static constexpr uint32_t kIa32SyscallMask = 0xC0000084;
/// 64bit FS base
static constexpr uint32_t kIa32FsBase = 0xC0000100;
/// 64bit GS base
static constexpr uint32_t kIa32GsBase = 0xC0000101;
/// SwapGS GS shadow
static constexpr uint32_t kIa32KernelGsBase = 0xC0000102;

/// 基本 MSR

/// Platform ID
static constexpr uint32_t kIa32PlatformId = 0x00000017;
/// APIC Base Address
static constexpr uint32_t kIa32ApicBase = 0x0000001B;
/// Feature Control
static constexpr uint32_t kIa32FeatureControl = 0x0000003A;
/// Time Stamp Counter
static constexpr uint32_t kIa32Tsc = 0x00000010;
/// Misc Enable
static constexpr uint32_t kIa32MiscEnable = 0x000001A0;

/// 性能监控相关 MSR

/// Performance Global Control
static constexpr uint32_t kIa32PerfGlobalCtrl = 0x0000038F;
/// Performance Global Status
static constexpr uint32_t kIa32PerfGlobalStatus = 0x0000038E;
/// Performance Global Overflow Control
static constexpr uint32_t kIa32PerfGlobalOvfCtrl = 0x00000390;

/// 内存类型范围寄存器 (MTRR)

/// MTRR Capabilities
static constexpr uint32_t kIa32MtrrCap = 0x000000FE;
/// MTRR Default Type
static constexpr uint32_t kIa32MtrrDefType = 0x000002FF;
/// MTRR Physical Base 0
static constexpr uint32_t kIa32MtrrPhysBase0 = 0x00000200;
/// MTRR Physical Mask 0
static constexpr uint32_t kIa32MtrrPhysMask0 = 0x00000201;

/// 调试相关 MSR

/// Debug Control
static constexpr uint32_t kIa32DebugCtl = 0x000001D9;
/// Last Branch From IP
static constexpr uint32_t kIa32LastBranchFromIp = 0x000001DB;
/// Last Branch To IP
static constexpr uint32_t kIa32LastBranchToIp = 0x000001DC;

/**
 * @brief APIC 相关 MSR 专用命名空间
 */
namespace apic {
/// APIC Base 地址 MSR
static constexpr uint32_t kBase = kIa32ApicBase;

/// x2APIC MSR 寄存器地址
/// x2APIC ID Register
static constexpr uint32_t kId = 0x802;
/// x2APIC Version Register
static constexpr uint32_t kVersion = 0x803;
/// Task Priority Register
static constexpr uint32_t kTpr = 0x808;
/// End of Interrupt Register
static constexpr uint32_t kEoi = 0x80B;
/// Spurious Interrupt Vector Register
static constexpr uint32_t kSivr = 0x80F;
/// Interrupt Command Register
static constexpr uint32_t kIcr = 0x830;
/// LVT Timer Register
static constexpr uint32_t kLvtTimer = 0x832;
/// LVT LINT0 Register
static constexpr uint32_t kLvtLint0 = 0x835;
/// LVT LINT1 Register
static constexpr uint32_t kLvtLint1 = 0x836;
/// LVT Error Register
static constexpr uint32_t kLvtError = 0x837;
/// Timer Initial Count Register
static constexpr uint32_t kTimerInitCount = 0x838;
/// Timer Current Count Register
static constexpr uint32_t kTimerCurrCount = 0x839;
/// Timer Divide Configuration Register
static constexpr uint32_t kTimerDivide = 0x83E;

/// APIC Base MSR 位域定义
namespace base {
/// BSP (Bootstrap Processor) 位
static constexpr uint64_t kBspBit = 1ULL << 8;
/// Global Enable 位
static constexpr uint64_t kGlobalEnableBit = 1ULL << 11;
/// x2APIC Enable 位
static constexpr uint64_t kX2ApicEnableBit = 1ULL << 10;
/// APIC Base 地址掩码
static constexpr uint64_t kBaseMask = 0xFFFFF000ULL;

}  // namespace base
}  // namespace apic
}  // namespace msr
}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_MSR_H_
