/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_X86_64_CPU_HPP_
#define CPU_IO_INCLUDE_X86_64_CPU_HPP_

#include <sys/cdefs.h>

#include <cstdint>
#include <type_traits>

#include "apic.hpp"
#include "context.hpp"
#include "cpuid.hpp"
#include "io.hpp"
#include "msr.h"
#include "pic.hpp"
#include "pit.hpp"
#include "regs.hpp"
#include "serial.hpp"
#include "virtual_memory.hpp"

/**
 * x86_64 cpu 相关定义
 * @note 寄存器读写设计见 arch/README.md
 * @see sdm.pdf
 * Intel® 64 and IA-32 Architectures Software Developer’s Manual
 * Volume 3 (3A, 3B, 3C, & 3D): System Programming Guide
 * Order Number: 325384-083US
 * https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
 */
namespace cpu_io {

using Rbp = detail::regs::Rbp;
using Msr = detail::regs::Msr;
using Rflags = detail::regs::Rflags;
using Gdtr = detail::regs::Gdtr;
using Ldtr = detail::regs::Ldtr;
using Idtr = detail::regs::Idtr;
using Tr = detail::regs::Tr;
using Cr0 = detail::regs::cr::Cr0;
using Cr2 = detail::regs::cr::Cr2;
using Cr3 = detail::regs::cr::Cr3;
using Cr4 = detail::regs::cr::Cr4;
using Cr8 = detail::regs::cr::Cr8;
using Xcr0 = detail::regs::Xcr0;
using Cs = detail::regs::segment_register::Cs;
using Ss = detail::regs::segment_register::Ss;
using Ds = detail::regs::segment_register::Ds;
using Es = detail::regs::segment_register::Es;
using Fs = detail::regs::segment_register::Fs;
using Gs = detail::regs::segment_register::Gs;

/// 中断上下文，由 cpu 自动压入，无错误码
struct InterruptContext {
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
};

/// 中断上下文，由 cpu 自动压入，有错误码
struct InterruptContextErrorCode {
  detail::register_info::IdtrInfo::ErrorCode error_code;
  uint32_t padding;
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
};

/**
 * @brief 允许中断
 */
static __always_inline void EnableInterrupt() { Rflags::If::Set(); }

/**
 * @brief 关闭中断
 */
static __always_inline void DisableInterrupt() { Rflags::If::Clear(); }

/**
 * @brief 获取中断状态
 */
static __always_inline auto GetInterruptStatus() -> bool {
  return Rflags::If::Get();
}

/**
 * @brief 获取当前 core id
 * @return size_t core id
 */
static __always_inline auto GetCurrentCoreId() -> size_t {
  return cpuid::GetExtendedApicId();
}

/**
 * @brief CPU 空转指令
 * @note x86_64 pause 指令
 */
static __always_inline void Pause() { __asm__ volatile("pause" ::: "memory"); }

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_CPU_HPP_
