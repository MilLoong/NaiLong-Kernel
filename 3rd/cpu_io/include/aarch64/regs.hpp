/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_AARCH64_REGS_HPP_
#define CPU_IO_INCLUDE_AARCH64_REGS_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <typeinfo>

#include "regs/read_write.hpp"
#include "regs/register_info.hpp"

/**
 * aarch64 cpu Control and Status Registers 相关定义
 * @see
 * https://developer.arm.com/documentation/#numberOfResults=48&&cf-navigationhierarchiesproducts=%20Architectures,CPU%20Architecture,A-Profile,Armv8-A
 * @note 寄存器读写设计见 arch/README.md
 */
namespace cpu_io {

namespace detail {

namespace regs {
struct X0 : public read_write::ReadWriteRegBase<register_info::X0Info> {};
struct X29 : public read_write::ReadWriteRegBase<register_info::X29Info> {};

namespace system_reg {

struct CPACR_EL1 : public read_write::ReadWriteRegBase<
                       register_info::system_reg::CPACR_EL1Info> {
  using Fpen = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::CPACR_EL1Info>,
      register_info::system_reg::CPACR_EL1Info::Fpen>;
};

struct CurrentEL : public read_write::ReadOnlyRegBase<
                       register_info::system_reg::CurrentELInfo> {
  using EL = read_write::ReadOnlyField<
      read_write::ReadOnlyRegBase<register_info::system_reg::CurrentELInfo>,
      register_info::system_reg::CurrentELInfo::EL>;
};

struct SPSel : public read_write::ReadWriteRegBase<
                   register_info::system_reg::SPSelInfo> {
  using SP = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::SPSelInfo>,
      register_info::system_reg::SPSelInfo::SP>;
};

struct DAIF
    : public read_write::ReadWriteRegBase<register_info::system_reg::DAIFInfo> {
  using D = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::DAIFInfo>,
      register_info::system_reg::DAIFInfo::D>;

  using A = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::DAIFInfo>,
      register_info::system_reg::DAIFInfo::A>;

  using I = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::DAIFInfo>,
      register_info::system_reg::DAIFInfo::I>;

  using F = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::DAIFInfo>,
      register_info::system_reg::DAIFInfo::F>;
};

struct VBAR_EL1 : public read_write::ReadWriteRegBase<
                      register_info::system_reg::VBAR_EL1Info> {
  using Base = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::VBAR_EL1Info>,
      register_info::system_reg::VBAR_EL1Info::Base>;
};

struct ELR_EL1 : public read_write::ReadWriteRegBase<
                     register_info::system_reg::ELR_EL1Info> {};

struct SPSR_EL1 : public read_write::ReadWriteRegBase<
                      register_info::system_reg::SPSR_EL1Info> {};

struct SP_EL0 : public read_write::ReadWriteRegBase<
                    register_info::system_reg::SP_EL0Info> {};

struct SP_EL1 : public read_write::ReadWriteRegBase<
                    register_info::system_reg::SP_EL1Info> {};

struct MPIDR_EL1 : public read_write::ReadOnlyRegBase<
                       register_info::system_reg::MPIDR_EL1Info> {
  using Aff3 = read_write::ReadOnlyField<
      read_write::ReadOnlyRegBase<register_info::system_reg::MPIDR_EL1Info>,
      register_info::system_reg::MPIDR_EL1Info::Aff3>;
  using U = read_write::ReadOnlyField<
      read_write::ReadOnlyRegBase<register_info::system_reg::MPIDR_EL1Info>,
      register_info::system_reg::MPIDR_EL1Info::U>;
  using MT = read_write::ReadOnlyField<
      read_write::ReadOnlyRegBase<register_info::system_reg::MPIDR_EL1Info>,
      register_info::system_reg::MPIDR_EL1Info::MT>;
  using Aff2 = read_write::ReadOnlyField<
      read_write::ReadOnlyRegBase<register_info::system_reg::MPIDR_EL1Info>,
      register_info::system_reg::MPIDR_EL1Info::Aff2>;
  using Aff1 = read_write::ReadOnlyField<
      read_write::ReadOnlyRegBase<register_info::system_reg::MPIDR_EL1Info>,
      register_info::system_reg::MPIDR_EL1Info::Aff1>;
  using Aff0 = read_write::ReadOnlyField<
      read_write::ReadOnlyRegBase<register_info::system_reg::MPIDR_EL1Info>,
      register_info::system_reg::MPIDR_EL1Info::Aff0>;
};

struct SCTLR_EL1 : public read_write::ReadWriteRegBase<
                       register_info::system_reg::SCTLR_EL1Info> {
  using M = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::SCTLR_EL1Info>,
      register_info::system_reg::SCTLR_EL1Info::M>;

  using C = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::SCTLR_EL1Info>,
      register_info::system_reg::SCTLR_EL1Info::C>;

  using I = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::SCTLR_EL1Info>,
      register_info::system_reg::SCTLR_EL1Info::I>;
};

struct MAIR_EL1 : public read_write::ReadWriteRegBase<
                      register_info::system_reg::MAIR_EL1Info> {
  using Aff7 = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::MAIR_EL1Info>,
      register_info::system_reg::MAIR_EL1Info::Aff7>;

  using Aff6 = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::MAIR_EL1Info>,
      register_info::system_reg::MAIR_EL1Info::Aff6>;

  using Aff5 = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::MAIR_EL1Info>,
      register_info::system_reg::MAIR_EL1Info::Aff5>;

  using Aff4 = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::MAIR_EL1Info>,
      register_info::system_reg::MAIR_EL1Info::Aff4>;

  using Aff3 = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::MAIR_EL1Info>,
      register_info::system_reg::MAIR_EL1Info::Aff3>;

  using Aff2 = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::MAIR_EL1Info>,
      register_info::system_reg::MAIR_EL1Info::Aff2>;

  using Aff1 = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::MAIR_EL1Info>,
      register_info::system_reg::MAIR_EL1Info::Aff1>;

  using Aff0 = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::MAIR_EL1Info>,
      register_info::system_reg::MAIR_EL1Info::Aff0>;
};

struct TCR_EL1 : public read_write::ReadWriteRegBase<
                     register_info::system_reg::TCR_EL1Info> {
  using IPS = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::TCR_EL1Info>,
      register_info::system_reg::TCR_EL1Info::IPS>;

  using TG1 = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::TCR_EL1Info>,
      register_info::system_reg::TCR_EL1Info::TG1>;

  using T1SZ = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::TCR_EL1Info>,
      register_info::system_reg::TCR_EL1Info::T1SZ>;

  using TG0 = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::TCR_EL1Info>,
      register_info::system_reg::TCR_EL1Info::TG0>;

  using T0SZ = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::TCR_EL1Info>,
      register_info::system_reg::TCR_EL1Info::T0SZ>;
};

struct ESR_EL1 : public read_write::ReadWriteRegBase<
                     register_info::system_reg::ESR_EL1Info> {
  using ISS2 = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::ESR_EL1Info>,
      register_info::system_reg::ESR_EL1Info::ISS2>;

  using EC = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::ESR_EL1Info>,
      register_info::system_reg::ESR_EL1Info::EC>;

  using ISS = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::ESR_EL1Info>,
      register_info::system_reg::ESR_EL1Info::ISS>;
};

struct FAR_EL1 : public read_write::ReadWriteRegBase<
                     register_info::system_reg::FAR_EL1Info> {};

struct CNTV_CTL_EL0 : public read_write::ReadWriteRegBase<
                          register_info::system_reg::CNTV_CTL_EL0Info> {
  using ISTATUS = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::CNTV_CTL_EL0Info>,
      register_info::system_reg::CNTV_CTL_EL0Info::ISTATUS>;

  using IMASK = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::CNTV_CTL_EL0Info>,
      register_info::system_reg::CNTV_CTL_EL0Info::IMASK>;

  using ENABLE = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::CNTV_CTL_EL0Info>,
      register_info::system_reg::CNTV_CTL_EL0Info::ENABLE>;
};

struct CNTV_TVAL_EL0 : public read_write::ReadWriteRegBase<
                           register_info::system_reg::CNTV_TVAL_EL0Info> {
  using TimerValue = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<
          register_info::system_reg::CNTV_TVAL_EL0Info>,
      register_info::system_reg::CNTV_TVAL_EL0Info::TimerValue>;
};

struct CNTVCT_EL0 : public read_write::ReadWriteRegBase<
                        register_info::system_reg::CNTVCT_EL0Info> {};

struct CNTFRQ_EL0 : public read_write::ReadWriteRegBase<
                        register_info::system_reg::CNTFRQ_EL0Info> {};

struct ICC_PMR_EL1 : public read_write::ReadWriteRegBase<
                         register_info::system_reg::ICC_PMR_EL1Info> {
  using Priority = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::ICC_PMR_EL1Info>,
      register_info::system_reg::ICC_PMR_EL1Info::Priority>;
};

struct ICC_IGRPEN1_EL1 : public read_write::ReadWriteRegBase<
                             register_info::system_reg::ICC_IGRPEN1_EL1Info> {
  using Enable = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<
          register_info::system_reg::ICC_IGRPEN1_EL1Info>,
      register_info::system_reg::ICC_IGRPEN1_EL1Info::Enable>;
};

struct ICC_SRE_EL1 : public read_write::ReadWriteRegBase<
                         register_info::system_reg::ICC_SRE_EL1Info> {
  using DIB = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::ICC_SRE_EL1Info>,
      register_info::system_reg::ICC_SRE_EL1Info::DIB>;

  using DFB = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::ICC_SRE_EL1Info>,
      register_info::system_reg::ICC_SRE_EL1Info::DFB>;

  using SRE = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::ICC_SRE_EL1Info>,
      register_info::system_reg::ICC_SRE_EL1Info::SRE>;
};

struct ICC_IAR1_EL1 : public read_write::ReadOnlyRegBase<
                          register_info::system_reg::ICC_IAR1_EL1Info> {
  using INTID = read_write::ReadOnlyField<
      read_write::ReadOnlyRegBase<register_info::system_reg::ICC_IAR1_EL1Info>,
      register_info::system_reg::ICC_IAR1_EL1Info::INTID>;
};

struct ICC_EOIR1_EL1 : public read_write::WriteOnlyRegBase<
                           register_info::system_reg::ICC_EOIR1_EL1Info> {
  using INTID = read_write::WriteOnlyField<
      read_write::WriteOnlyRegBase<
          register_info::system_reg::ICC_EOIR1_EL1Info>,
      register_info::system_reg::ICC_EOIR1_EL1Info::INTID>;
};

struct ICC_SGI1R_EL1 : public read_write::WriteOnlyRegBase<
                           register_info::system_reg::ICC_SGI1R_EL1Info> {
  using Aff3 = read_write::WriteOnlyField<
      read_write::WriteOnlyRegBase<
          register_info::system_reg::ICC_SGI1R_EL1Info>,
      register_info::system_reg::ICC_SGI1R_EL1Info::Aff3>;
  
  using RS = read_write::WriteOnlyField<
      read_write::WriteOnlyRegBase<
          register_info::system_reg::ICC_SGI1R_EL1Info>,
      register_info::system_reg::ICC_SGI1R_EL1Info::RS>;
  
  using IRM = read_write::WriteOnlyField<
      read_write::WriteOnlyRegBase<
          register_info::system_reg::ICC_SGI1R_EL1Info>,
      register_info::system_reg::ICC_SGI1R_EL1Info::IRM>;
  
  using Aff2 = read_write::WriteOnlyField<
      read_write::WriteOnlyRegBase<
          register_info::system_reg::ICC_SGI1R_EL1Info>,
      register_info::system_reg::ICC_SGI1R_EL1Info::Aff2>;
  
  using INTID = read_write::WriteOnlyField<
      read_write::WriteOnlyRegBase<
          register_info::system_reg::ICC_SGI1R_EL1Info>,
      register_info::system_reg::ICC_SGI1R_EL1Info::INTID>;
  
  using Aff1 = read_write::WriteOnlyField<
      read_write::WriteOnlyRegBase<
          register_info::system_reg::ICC_SGI1R_EL1Info>,
      register_info::system_reg::ICC_SGI1R_EL1Info::Aff1>;
  
  using TargetList = read_write::WriteOnlyField<
      read_write::WriteOnlyRegBase<
          register_info::system_reg::ICC_SGI1R_EL1Info>,
      register_info::system_reg::ICC_SGI1R_EL1Info::TargetList>;
};

struct TTBR0_EL1 : public read_write::ReadWriteRegBase<
                       register_info::system_reg::TTBR0_EL1Info> {
  using ASID = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::TTBR0_EL1Info>,
      register_info::system_reg::TTBR0_EL1Info::ASID>;

  using BADDR = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::TTBR0_EL1Info>,
      register_info::system_reg::TTBR0_EL1Info::BADDR>;

  using CnP = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::TTBR0_EL1Info>,
      register_info::system_reg::TTBR0_EL1Info::CnP>;
};

struct TTBR1_EL1 : public read_write::ReadWriteRegBase<
                       register_info::system_reg::TTBR1_EL1Info> {
  using ASID = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::TTBR1_EL1Info>,
      register_info::system_reg::TTBR1_EL1Info::ASID>;

  using BADDR = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::TTBR1_EL1Info>,
      register_info::system_reg::TTBR1_EL1Info::BADDR>;

  using CnP = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::system_reg::TTBR1_EL1Info>,
      register_info::system_reg::TTBR1_EL1Info::CnP>;
};

}  // namespace system_reg

}  // namespace regs

}  // namespace detail

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_AARCH64_REGS_HPP_
