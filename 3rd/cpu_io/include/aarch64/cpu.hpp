/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_AARCH64_CPU_HPP_
#define CPU_IO_INCLUDE_AARCH64_CPU_HPP_

#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <typeinfo>

#include "context.hpp"
#include "psci.hpp"
#include "regs.hpp"
#include "virtual_memory.hpp"

/**
 * aarch64 cpu 相关定义
 * @note 寄存器读写设计见 arch/README.md
 */
namespace cpu_io {

using X0 = detail::regs::X0;
using X29 = detail::regs::X29;
using CPACR_EL1 = detail::regs::system_reg::CPACR_EL1;
using CurrentEL = detail::regs::system_reg::CurrentEL;
using SPSel = detail::regs::system_reg::SPSel;
using DAIF = detail::regs::system_reg::DAIF;
using VBAR_EL1 = detail::regs::system_reg::VBAR_EL1;
using ELR_EL1 = detail::regs::system_reg::ELR_EL1;
using SPSR_EL1 = detail::regs::system_reg::SPSR_EL1;
using SP_EL0 = detail::regs::system_reg::SP_EL0;
using SP_EL1 = detail::regs::system_reg::SP_EL1;
using MPIDR_EL1 = detail::regs::system_reg::MPIDR_EL1;
using SCTLR_EL1 = detail::regs::system_reg::SCTLR_EL1;
using MAIR_EL1 = detail::regs::system_reg::MAIR_EL1;
using TCR_EL1 = detail::regs::system_reg::TCR_EL1;
using TTBR0_EL1 = detail::regs::system_reg::TTBR0_EL1;
using TTBR1_EL1 = detail::regs::system_reg::TTBR1_EL1;
using ESR_EL1 = detail::regs::system_reg::ESR_EL1;
using FAR_EL1 = detail::regs::system_reg::FAR_EL1;
using CNTV_CTL_EL0 = detail::regs::system_reg::CNTV_CTL_EL0;
using CNTV_TVAL_EL0 = detail::regs::system_reg::CNTV_TVAL_EL0;
using CNTVCT_EL0 = detail::regs::system_reg::CNTVCT_EL0;
using CNTFRQ_EL0 = detail::regs::system_reg::CNTFRQ_EL0;
using ICC_PMR_EL1 = detail::regs::system_reg::ICC_PMR_EL1;
using ICC_IGRPEN1_EL1 = detail::regs::system_reg::ICC_IGRPEN1_EL1;
using ICC_SRE_EL1 = detail::regs::system_reg::ICC_SRE_EL1;
using ICC_IAR1_EL1 = detail::regs::system_reg::ICC_IAR1_EL1;
using ICC_EOIR1_EL1 = detail::regs::system_reg::ICC_EOIR1_EL1;
using ICC_SGI1R_EL1 = detail::regs::system_reg::ICC_SGI1R_EL1;

/**
 * @brief 允许中断
 * @todo
 */
static __always_inline void EnableInterrupt() {
  DAIF::D::Clear();
  DAIF::A::Clear();
  DAIF::I::Clear();
  DAIF::F::Clear();
}

/**
 * @brief 关闭中断
 * @todo
 */
static __always_inline void DisableInterrupt() {
  DAIF::D::Set();
  DAIF::A::Set();
  DAIF::I::Set();
  DAIF::F::Set();
}

/**
 * @brief 获取中断状态
 * @return true 允许中断
 * @return false 禁用中断
 */
static __always_inline auto GetInterruptStatus() -> bool {
  return !DAIF::I::Get() && !DAIF::F::Get();
}

/**
 * @brief 获取当前 core id
 * @return size_t core id
 */
static __always_inline auto GetCurrentCoreId() -> size_t {
  return MPIDR_EL1::Aff0::Get();
}

/**
 * @brief 初始化 FPU
 */
static __always_inline void SetupFpu() { CPACR_EL1::Fpen::Set(); }

/**
 * @brief CPU 空转指令
 * @note AArch64 yield hint 指令
 */
static __always_inline void Pause() { __asm__ volatile("yield" ::: "memory"); }

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_AARCH64_CPU_HPP_
