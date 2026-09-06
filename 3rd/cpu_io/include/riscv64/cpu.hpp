/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_RISCV64_CPU_HPP_
#define CPU_IO_INCLUDE_RISCV64_CPU_HPP_

#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <typeinfo>

#include "context.hpp"
#include "regs.hpp"
#include "virtual_memory.hpp"

namespace cpu_io {

using Fp = detail::regs::Fp;
using Tp = detail::regs::Tp;
using Sstatus = detail::regs::csr::Sstatus;
using Stvec = detail::regs::csr::Stvec;
using Sip = detail::regs::csr::Sip;
using Sie = detail::regs::csr::Sie;
using Time = detail::regs::csr::Time;
using Cycle = detail::regs::csr::Cycle;
using Instret = detail::regs::csr::Instret;
using Sscratch = detail::regs::csr::Sscratch;
using Sepc = detail::regs::csr::Sepc;
using Scause = detail::regs::csr::Scause;
using Stval = detail::regs::csr::Stval;
using Satp = detail::regs::csr::Satp;
using Stimecmp = detail::regs::csr::Stimecmp;

/**
 * @brief 允许中断
 */
static __always_inline void EnableInterrupt() { Sstatus::Sie::Set(); }

/**
 * @brief 关闭中断
 */
static __always_inline void DisableInterrupt() { Sstatus::Sie::Clear(); }

/**
 * @brief 获取中断状态
 * @return bool 中断状态
 */
static __always_inline auto GetInterruptStatus() -> bool {
  return Sstatus::Sie::Get();
}

/**
 * @brief 获取当前 core id
 * @return size_t core id
 */
static __always_inline auto GetCurrentCoreId() -> size_t { return Tp::Read(); }

/**
 * @brief CPU 空转指令
 * @note RISC-V pause hint 指令
 */
static __always_inline void Pause() {
  __asm__ volatile("pause" ::: "memory");
}

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_RISCV64_CPU_HPP_
