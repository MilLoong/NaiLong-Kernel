/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_RISCV64_REGS_HPP_
#define CPU_IO_INCLUDE_RISCV64_REGS_HPP_

#include <sys/cdefs.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

#include "regs/read_write.hpp"
#include "regs/register_info.hpp"

/**
 * riscv64 cpu Control and Status Registers 相关定义
 * @note 寄存器读写设计见 arch/README.md
 * @see priv-isa.pdf
 * https://github.com/riscv/riscv-isa-manual/releases/tag/20240411/priv-isa-asciidoc.pdf
 * @see riscv-abi.pdf
 * https://github.com/riscv-non-isa/riscv-elf-psabi-doc/releases/tag/v1.0
 * @see
 * https://github.com/five-embeddev/riscv-scratchpad/blob/master/baremetal-startup-cxx/src/riscv-csr.hpp
 */
namespace cpu_io {

namespace detail {

namespace regs {
struct Fp : public read_write::ReadWriteRegBase<register_info::FpInfo> {};
struct Tp : public read_write::ReadWriteRegBase<register_info::TpInfo> {};

namespace csr {

struct Sstatus
    : public read_write::ReadWriteRegBase<register_info::csr::SstatusInfo> {
  using Sie = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SstatusInfo>,
      register_info::csr::SstatusInfo::Sie>;
  using Spie = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SstatusInfo>,
      register_info::csr::SstatusInfo::Spie>;
  using Spp = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SstatusInfo>,
      register_info::csr::SstatusInfo::Spp>;
};

struct Stvec
    : public read_write::ReadWriteRegBase<register_info::csr::StvecInfo> {
  using Base = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::StvecInfo>,
      register_info::csr::StvecInfo::Base>;
  using Mode = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::StvecInfo>,
      register_info::csr::StvecInfo::Mode>;

  static __always_inline bool SetDirect(uint64_t addr) {
    // 地址需 4 字节对齐
    if ((addr & 0x3) != 0) {
      return false;
    }
    Base::Write(addr);
    Mode::Write(register_info::csr::StvecInfo::kDirect);
    return true;
  }
};

struct Sip : public read_write::ReadWriteRegBase<register_info::csr::SipInfo> {
  using Ssip = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SipInfo>,
      register_info::csr::SipInfo::Ssip>;
  using Stip = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SipInfo>,
      register_info::csr::SipInfo::Stip>;
  using Seip = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SipInfo>,
      register_info::csr::SipInfo::Seip>;
};

struct Sie : public read_write::ReadWriteRegBase<register_info::csr::SieInfo> {
  using Ssie = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SieInfo>,
      register_info::csr::SieInfo::Ssie>;
  using Stie = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SieInfo>,
      register_info::csr::SieInfo::Stie>;
  using Seie = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SieInfo>,
      register_info::csr::SieInfo::Seie>;
};

struct Time : public read_write::ReadOnlyRegBase<register_info::csr::TimeInfo> {
};

struct Cycle
    : public read_write::ReadOnlyRegBase<register_info::csr::CycleInfo> {};

struct Instret
    : public read_write::ReadOnlyRegBase<register_info::csr::InstretInfo> {};

struct Sscratch
    : public read_write::ReadWriteRegBase<register_info::csr::SscratchInfo> {};

struct Sepc
    : public read_write::ReadWriteRegBase<register_info::csr::SepcInfo> {};

struct Scause
    : public read_write::ReadWriteRegBase<register_info::csr::ScauseInfo> {
  using Interrupt = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::ScauseInfo>,
      register_info::csr::ScauseInfo::Interrupt>;
  using ExceptionCode = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::ScauseInfo>,
      register_info::csr::ScauseInfo::ExceptionCode>;
};

struct Stval
    : public read_write::ReadWriteRegBase<register_info::csr::StvalInfo> {};

struct Satp
    : public read_write::ReadWriteRegBase<register_info::csr::SatpInfo> {
  using Ppn = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SatpInfo>,
      register_info::csr::SatpInfo::Ppn>;
  using Asid = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SatpInfo>,
      register_info::csr::SatpInfo::Asid>;
  using Mode = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SatpInfo>,
      register_info::csr::SatpInfo::Mode>;
};

struct Stimecmp
    : public read_write::ReadOnlyRegBase<register_info::csr::StimecmpInfo> {};

}  // namespace csr

}  // namespace regs

}  // namespace detail

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_RISCV64_REGS_HPP_
