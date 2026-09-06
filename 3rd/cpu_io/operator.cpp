
/**
 * @file main.cpp
 * @brief 用于测试 c++ 初始化和异常支持
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2023-07-15
 * @copyright MIT LICENSE
 * https://github.com/Simple-XX/SimpleKernel
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2023-07-15<td>Zone.N (Zone.Niuzh@hotmail.com)<td>创建文件
 * </table>
 */

#include <iostream>

#include "include/cpu_io.h"

#ifdef __x86_64__

auto operator<<(std::ostream &ostream, const Cr8 &) -> std::ostream & {
  printf("val: 0x%p", reinterpret_cast<void *>(cpu::regs::cr::Cr8::Read()));
  return ostream;
}

auto operator<<(std::ostream &ostream, const Cpuid &) -> std::ostream & {
  printf("val: 0x%p", reinterpret_cast<void *>(cpu::regs::Cpuid::Read()));
  return ostream;
}

auto operator<<(std::ostream &ostream, const Xcr0 &) -> std::ostream & {
  printf("val: 0x%p", reinterpret_cast<void *>(cpu::regs::Xcr0::Read()));
  return ostream;
}

auto operator<<(std::ostream &ostream, const Cs &) -> std::ostream & {
  auto val = cpu::regs::segment_register::Cs::Read();
  auto rpl = read_write::ReadOnlyField<
      read_write::ReadWriteRegBase<register_info::segment_register::CsInfo>,
      register_info::segment_register::CsInfo::Rpl>::Get();
  auto ti = read_write::ReadOnlyField<
      read_write::ReadWriteRegBase<register_info::segment_register::CsInfo>,
      register_info::segment_register::CsInfo::Ti>::Get();
  auto index = read_write::ReadOnlyField<
      read_write::ReadWriteRegBase<register_info::segment_register::CsInfo>,
      register_info::segment_register::CsInfo::Index>::Get();
  printf("val: 0x%X, rpl: %d, ti: %s, index: 0x%X", val, rpl,
         ti ? "LDT" : "GDT", index);
  return ostream;
}

auto operator<<(std::ostream &ostream, const Ss &) -> std::ostream & {
  auto val = cpu::regs::segment_register::Ss::Read();
  auto rpl = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::SsInfo>,
      register_info::segment_register::SsInfo::Rpl>::Get();
  auto ti = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::SsInfo>,
      register_info::segment_register::SsInfo::Ti>::Get();
  auto index = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::SsInfo>,
      register_info::segment_register::SsInfo::Index>::Get();
  printf("val: 0x%X, rpl: %d, ti: %s, index: 0x%X", val, rpl,
         ti ? "LDT" : "GDT", index);
  return ostream;
}

auto operator<<(std::ostream &ostream, const Ds &) -> std::ostream & {
  auto val = cpu::regs::segment_register::Ds::Read();
  auto rpl = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::DsInfo>,
      register_info::segment_register::DsInfo::Rpl>::Get();
  auto ti = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::DsInfo>,
      register_info::segment_register::DsInfo::Ti>::Get();
  auto index = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::DsInfo>,
      register_info::segment_register::DsInfo::Index>::Get();
  printf("val: 0x%X, rpl: %d, ti: %s, index: 0x%X", val, rpl,
         ti ? "LDT" : "GDT", index);
  return ostream;
}

auto operator<<(std::ostream &ostream, const Es &) -> std::ostream & {
  auto val = cpu::regs::segment_register::Es::Read();
  auto rpl = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::EsInfo>,
      register_info::segment_register::EsInfo::Rpl>::Get();
  auto ti = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::EsInfo>,
      register_info::segment_register::EsInfo::Ti>::Get();
  auto index = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::EsInfo>,
      register_info::segment_register::EsInfo::Index>::Get();
  printf("val: 0x%X, rpl: %d, ti: %s, index: 0x%X", val, rpl,
         ti ? "LDT" : "GDT", index);
  return ostream;
}

auto operator<<(std::ostream &ostream, const Fs &) -> std::ostream & {
  auto val = cpu::regs::segment_register::Fs::Read();
  auto rpl = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::FsInfo>,
      register_info::segment_register::FsInfo::Rpl>::Get();
  auto ti = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::FsInfo>,
      register_info::segment_register::FsInfo::Ti>::Get();
  auto index = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::FsInfo>,
      register_info::segment_register::FsInfo::Index>::Get();
  printf("val: 0x%X, rpl: %d, ti: %s, index: 0x%X", val, rpl,
         ti ? "LDT" : "GDT", index);
  return ostream;
}

auto operator<<(std::ostream &ostream, const Gs &) -> std::ostream & {
  auto val = cpu::regs::segment_register::Gs::Read();
  auto rpl = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::GsInfo>,
      register_info::segment_register::GsInfo::Rpl>::Get();
  auto ti = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::GsInfo>,
      register_info::segment_register::GsInfo::Ti>::Get();
  auto index = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::GsInfo>,
      register_info::segment_register::GsInfo::Index>::Get();
  printf("val: 0x%X, rpl: %d, ti: %s, index: 0x%X", val, rpl,
         ti ? "LDT" : "GDT", index);
  return ostream;
}

auto operator<<(std::ostream &ostream, const Cr3 &) -> std::ostream & {
  auto pwt = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr3Info>,
      register_info::cr::Cr3Info::Pwt>::Get();
  auto pcd = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr3Info>,
      register_info::cr::Cr3Info::Pcd>::Get();
  auto page_directory_base = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr3Info>,
      register_info::cr::Cr3Info::PageDirectoryBase>::Get();
  printf("val: 0x%p, pwt: %s, pcd: %s, page_directory_base: 0x%p",
         reinterpret_cast<void *>(cpu::regs::cr::Cr3::Read()),
         (pwt ? "Enable" : "Disable"), (pcd ? "Enable" : "Disable"),
         reinterpret_cast<void *>(page_directory_base));
  return ostream;
}

auto operator<<(std::ostream &ostream, const Cr4 &) -> std::ostream & {
  auto pae = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr4Info>,
      register_info::cr::Cr4Info::Pae>::Get();
  printf("val: 0x%p, pae: %s",
         reinterpret_cast<void *>(cpu::regs::cr::Cr4::Read()),
         (pae ? "Enable" : "Disable"));
  return ostream;
}

auto operator<<(std::ostream &ostream,
                const SegmentDescriptor &segment_descriptor) -> std::ostream & {
  printf(
      "val: 0x%p, type: 0x%X, s: %s, dpl: 0x%X, p: %s, avl: %s, l: %s",
      reinterpret_cast<void *>(segment_descriptor.val),
      static_cast<uint8_t>(segment_descriptor.segment_descriptor.type),
      segment_descriptor.segment_descriptor.s ? "CodeData" : "System",
      static_cast<uint8_t>(segment_descriptor.segment_descriptor.dpl),
      segment_descriptor.segment_descriptor.p ? "Present" : "NotPresent",
      segment_descriptor.segment_descriptor.avl ? "Available" : "NotAvailable",
      segment_descriptor.segment_descriptor.l ? "64Bit" : "Legacy");
  return ostream;
}
auto operator<<(std::ostream &ostream, const ErrorCode &error_code)
    -> std::ostream & {
  printf(
      "val: 0x%X, ext: %d, idt: %d, ti: %d, segment_selector_index: "
      "0x%X",
      error_code.val, static_cast<uint8_t>(error_code.error_code.ext),
      static_cast<uint8_t>(error_code.error_code.idt),
      static_cast<uint8_t>(error_code.error_code.ti),
      static_cast<uint32_t>(error_code.error_code.segment_selector_index));
  return ostream;
}

auto operator<<(std::ostream &ostream, const Idt &idt) -> std::ostream & {
  printf(
      "val: 0x%p 0x%p, offset: 0x%p, selector: 0x%X, type: %d, dpl: 0x%X, "
      "p: %s",
      reinterpret_cast<void *>(idt.val[0]),
      reinterpret_cast<void *>(idt.val[1]),
      reinterpret_cast<void *>((idt.idt.offset3 << 32) |
                               (idt.idt.offset2 << 16) | idt.idt.offset1),
      idt.idt.selector, idt.idt.type, idt.idt.dpl,
      idt.idt.p ? "Present" : "NotPresent");
  return ostream;
}

auto operator<<(std::ostream &ostream, [[maybe_unused]] const Rbp &rbp)
    -> std::ostream & {
  printf("val: 0x%p", regs::Rbp::Read());
  return ostream;
}

auto operator<<(std::ostream &ostream, const Efer & /*efer*/)
    -> std::ostream & {
  auto sce = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::EferInfo>,
      register_info::EferInfo::Sce>::Get();
  auto lme = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::EferInfo>,
      register_info::EferInfo::Lme>::Get();
  auto lma = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::EferInfo>,
      register_info::EferInfo::Lma>::Get();
  auto nxe = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::EferInfo>,
      register_info::EferInfo::Nxe>::Get();
  printf("val: 0x%p, sce: %s, lme: %s, lma: %s, nxe: %s",
         reinterpret_cast<void *>(cpu::regs::Efer::Read()),
         (sce ? "Enable" : "Disable"), (lme ? "Enable" : "Disable"),
         (lma ? "Enable" : "Disable"), (nxe ? "Enable" : "Disable")

  );
  return ostream;
}
auto operator<<(std::ostream &ostream, const Rflags &) -> std::ostream & {
  auto interrupt_enable_flag = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::RflagsInfo>,
      register_info::RflagsInfo::If>::Get();
  printf("val: 0x%p, if: %s",
         reinterpret_cast<void *>(cpu::regs::Rflags::Read()),
         (interrupt_enable_flag ? "Enable" : "Disable"));
  return ostream;
}

auto operator<<(std::ostream &ostream, const Gdtr &) -> std::ostream & {
  printf("base: 0x%p, limit: %d",
         reinterpret_cast<void *>(cpu::regs::Gdtr::Read().base),
         cpu::regs::Gdtr::Read().limit);
  return ostream;
}

auto operator<<(std::ostream &ostream, const Ldtr &) -> std::ostream & {
  printf("val: 0x%p", reinterpret_cast<void *>(cpu::regs::Ldtr::Read()));
  return ostream;
}
auto operator<<(std::ostream &ostream, const Idtr &) -> std::ostream & {
  printf("base: 0x%p, limit: %d",
         reinterpret_cast<void *>(cpu::regs::Idtr::Read().base),
         cpu::regs::Idtr::Read().limit);
  return ostream;
}

auto operator<<(std::ostream &ostream, const Tr &) -> std::ostream & {
  printf("val: 0x%p", reinterpret_cast<void *>(cpu::regs::Tr::Read()));
  return ostream;
}

auto operator<<(std::ostream &ostream, const Cr0 &) -> std::ostream & {
  auto pe = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr0Info>,
      register_info::cr::Cr0Info::Pe>::Get();
  auto pg = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr0Info>,
      register_info::cr::Cr0Info::Pg>::Get();
  printf("val: 0x%p, pe: %s, pg: %s",
         reinterpret_cast<void *>(cpu::regs::cr::Cr0::Read()),
         (pe ? "Enable" : "Disable"), (pg ? "Enable" : "Disable")

  );
  return ostream;
}
auto operator<<(std::ostream &ostream, const Cr2 &) -> std::ostream & {
  printf("val: 0x%p", reinterpret_cast<void *>(cpu::regs::cr::Cr2::Read()));
  return ostream;
}

std::ostream &operator<<(std::ostream &os,
                         const InterruptContext &interrupt_context) {
  printf("rip: 0x%lX\n", interrupt_context.rip);
  printf("cs: 0x%lX\n", interrupt_context.cs);
  printf("rflags: 0x%lX\n", interrupt_context.rflags);
  printf("rsp: 0x%lX\n", interrupt_context.rsp);
  printf("ss: 0x%lX", interrupt_context.ss);
  return os;
}

std::ostream &operator<<(
    std::ostream &os,
    const InterruptContextErrorCode &interrupt_context_error_code) {
  std::cout << std::endl
            << interrupt_context_error_code.error_code << std::endl;
  printf("padding: 0x%X\n", interrupt_context_error_code.padding);
  printf("rip: 0x%lX\n", interrupt_context_error_code.rip);
  printf("cs: 0x%lX\n", interrupt_context_error_code.cs);
  printf("rflags: 0x%lX\n", interrupt_context_error_code.rflags);
  printf("rsp: 0x%lX\n", interrupt_context_error_code.rsp);
  printf("ss: 0x%lX", interrupt_context_error_code.ss);
  return os;
}

#elif __riscv

auto operator<<(std::ostream &ostream, [[maybe_unused]] const Fp &fp_reg)
    -> std::ostream & {
  printf("val: 0x%p", regs::Fp::Read());
  return ostream;
}

auto operator<<(std::ostream &ostream, const Sstatus &) -> std::ostream & {
  auto sie = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SstatusInfo>,
      register_info::csr::SstatusInfo::Sie>::Get();
  auto spie = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SstatusInfo>,
      register_info::csr::SstatusInfo::Spie>::Get();
  auto spp = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SstatusInfo>,
      register_info::csr::SstatusInfo::Spp>::Get();
  printf("val: 0x%p, sie: %s, spie: %s, spp: %s",
         reinterpret_cast<void *>(cpu::regs::csr::Sstatus::Read()),
         (sie ? "Enable" : "Disable"), (spie ? "Enable" : "Disable"),
         (spp ? "S Mode" : "U Mode")

  );
  return ostream;
}
auto operator<<(std::ostream &ostream, const Stvec &) -> std::ostream & {
  auto mode = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::StvecInfo>,
      register_info::csr::StvecInfo::Mode>::Get();
  auto base = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::StvecInfo>,
      register_info::csr::StvecInfo::Base>::Get();
  printf(
      "val: 0x%p, mode: %s, base: 0x%lX",
      reinterpret_cast<void *>(cpu::regs::csr::Stvec::Read()),
      (mode == register_info::csr::StvecInfo::kDirect ? "Direct" : "Vectored"),
      base);
  return ostream;
}
auto operator<<(std::ostream &ostream, const Sip &) -> std::ostream & {
  auto ssip = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SipInfo>,
      register_info::csr::SipInfo::Ssip>::Get();
  auto stip = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SipInfo>,
      register_info::csr::SipInfo::Stip>::Get();
  auto seip = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SipInfo>,
      register_info::csr::SipInfo::Seip>::Get();
  printf("val: 0x%p, ssie: %s, stie: %s, seie: %s",
         reinterpret_cast<void *>(cpu::regs::csr::Sip::Read()),
         (ssip ? "Enable" : "Disable"), (stip ? "Enable" : "Disable"),
         (seip ? "Enable" : "Disable"));
  return ostream;
}
auto operator<<(std::ostream &ostream, const Sie &) -> std::ostream & {
  auto ssie = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SieInfo>,
      register_info::csr::SieInfo::Ssie>::Get();
  auto stie = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SieInfo>,
      register_info::csr::SieInfo::Stie>::Get();
  auto seie = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SieInfo>,
      register_info::csr::SieInfo::Seie>::Get();
  printf("val: 0x%p, ssie: %s, stie: %s, seie: %s",
         reinterpret_cast<void *>(cpu::regs::csr::Sie::Read()),
         (ssie ? "Enable" : "Disable"), (stie ? "Enable" : "Disable"),
         (seie ? "Enable" : "Disable"));
  return ostream;
}
auto operator<<(std::ostream &ostream, const Time &) -> std::ostream & {
  printf("val: 0x%p", reinterpret_cast<void *>(cpu::regs::csr::Time::Read()));
  return ostream;
}
auto operator<<(std::ostream &ostream, const Cycle &) -> std::ostream & {
  printf("val: 0x%p", reinterpret_cast<void *>(cpu::regs::csr::Cycle::Read()));
  return ostream;
}
auto operator<<(std::ostream &ostream, const Instret &) -> std::ostream & {
  printf("val: 0x%p",
         reinterpret_cast<void *>(cpu::regs::csr::Instret::Read()));
  return ostream;
}
auto operator<<(std::ostream &ostream, const Sscratch &) -> std::ostream & {
  printf("val: 0x%p",
         reinterpret_cast<void *>(cpu::regs::csr::Sscratch::Read()));
  return ostream;
}
auto operator<<(std::ostream &ostream, const Sepc &) -> std::ostream & {
  printf("val: 0x%p", reinterpret_cast<void *>(cpu::regs::csr::Sepc::Read()));
  return ostream;
}
auto operator<<(std::ostream &ostream, const Scause &) -> std::ostream & {
  auto exception_code = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::ScauseInfo>,
      register_info::csr::ScauseInfo::ExceptionCode>::Get();
  auto interrupt = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::ScauseInfo>,
      register_info::csr::ScauseInfo::Interrupt>::Get();
  printf("val: 0x%p, exception_code: 0x%lX, interrupt: %s, name: %s",
         reinterpret_cast<void *>(cpu::regs::csr::Scause::Read()),
         exception_code, interrupt ? "Yes" : "No",
         interrupt
             ? register_info::csr::ScauseInfo::kInterruptNames[exception_code]
             : register_info::csr::ScauseInfo::kExceptionNames[exception_code]);
  return ostream;
}
auto operator<<(std::ostream &ostream, const Stval &) -> std::ostream & {
  printf("val: 0x%p", reinterpret_cast<void *>(cpu::regs::csr::Stval::Read()));
  return ostream;
}
auto operator<<(std::ostream &ostream, const Satp &) -> std::ostream & {
  auto ppn = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SatpInfo>,
      register_info::csr::SatpInfo::Ppn>::Get();
  auto asid = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SatpInfo>,
      register_info::csr::SatpInfo::Asid>::Get();
  auto mode = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::csr::SatpInfo>,
      register_info::csr::SatpInfo::Mode>::Get();
  printf("val: 0x%p, ppn: 0x%lX, asid: 0x%X, mode: %s",
         reinterpret_cast<void *>(cpu::regs::csr::Satp::Read()), ppn, asid,
         register_info::csr::SatpInfo::kModeNames[mode]);
  return ostream;
}
auto operator<<(std::ostream &ostream, const Stimecmp &) -> std::ostream & {
  printf("val: 0x%p",
         reinterpret_cast<void *>(cpu::regs::csr::Stimecmp::Read()));
  return ostream;
}
#elif __aarch64__
friend auto operator<<(nk_std::ostream &os, [[maybe_unused]] const X29 &x29)
    -> nk_std::ostream & {
  CPU_IO_PRINTF("val: 0x%p", regs::X29::Read());
  return os;
}
#endif
