/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_X86_64_REGS_HPP_
#define CPU_IO_INCLUDE_X86_64_REGS_HPP_

#include <sys/cdefs.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

#include "regs/read_write.hpp"
#include "regs/register_info.hpp"

namespace cpu_io {

namespace detail {

namespace regs {

struct Rbp : public read_write::ReadWriteRegBase<register_info::RbpInfo> {};

struct Msr : public read_write::ReadWriteRegBase<register_info::MsrInfo> {};

struct Rflags : public read_write::ReadWriteRegBase<register_info::RflagsInfo> {
  using If = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::RflagsInfo>,
      register_info::RflagsInfo::If>;
};

struct Gdtr : public read_write::ReadWriteRegBase<register_info::GdtrInfo> {
  using Limit = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::GdtrInfo>,
      register_info::GdtrInfo::Limit>;
  using Base = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::GdtrInfo>,
      register_info::GdtrInfo::Base>;
};

struct Ldtr : public read_write::ReadWriteRegBase<register_info::LdtrInfo> {};

struct Idtr : public read_write::ReadWriteRegBase<register_info::IdtrInfo> {
  using Limit = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::IdtrInfo>,
      register_info::IdtrInfo::Limit>;
  using Base = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::IdtrInfo>,
      register_info::IdtrInfo::Base>;
};

struct Tr : public read_write::ReadWriteRegBase<register_info::TrInfo> {};

namespace cr {

struct Cr0 : public read_write::ReadWriteRegBase<register_info::cr::Cr0Info> {
  using Pe = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr0Info>,
      register_info::cr::Cr0Info::Pe>;
  using Pg = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr0Info>,
      register_info::cr::Cr0Info::Pg>;
};

struct Cr2 : public read_write::ReadWriteRegBase<register_info::cr::Cr2Info> {};

struct Cr3 : public read_write::ReadWriteRegBase<register_info::cr::Cr3Info> {
  using Pwt = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr3Info>,
      register_info::cr::Cr3Info::Pwt>;
  using Pcd = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr3Info>,
      register_info::cr::Cr3Info::Pcd>;
  using PageDirectoryBase = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr3Info>,
      register_info::cr::Cr3Info::PageDirectoryBase>;
};

struct Cr4 : public read_write::ReadWriteRegBase<register_info::cr::Cr4Info> {
  using Pae = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::cr::Cr4Info>,
      register_info::cr::Cr4Info::Pae>;
};

struct Cr8 : public read_write::ReadWriteRegBase<register_info::cr::Cr8Info> {};

}  // namespace cr

struct Xcr0 : public read_write::ReadWriteRegBase<register_info::Xcr0Info> {};

namespace segment_register {
struct Cs : public read_write::ReadWriteRegBase<
                register_info::segment_register::CsInfo> {
  using Rpl = read_write::ReadOnlyField<
      read_write::ReadWriteRegBase<register_info::segment_register::CsInfo>,
      register_info::segment_register::CsInfo::Rpl>;

  using Ti = read_write::ReadOnlyField<
      read_write::ReadWriteRegBase<register_info::segment_register::CsInfo>,
      register_info::segment_register::CsInfo::Ti>;

  using Index = read_write::ReadOnlyField<
      read_write::ReadWriteRegBase<register_info::segment_register::CsInfo>,
      register_info::segment_register::CsInfo::Index>;
};

struct Ss : public read_write::ReadWriteRegBase<
                register_info::segment_register::SsInfo> {
  using Rpl = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::SsInfo>,
      register_info::segment_register::SsInfo::Rpl>;

  using Ti = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::SsInfo>,
      register_info::segment_register::SsInfo::Ti>;

  using Index = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::SsInfo>,
      register_info::segment_register::SsInfo::Index>;
};

struct Ds : public read_write::ReadWriteRegBase<
                register_info::segment_register::DsInfo> {
  using Rpl = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::DsInfo>,
      register_info::segment_register::DsInfo::Rpl>;

  using Ti = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::DsInfo>,
      register_info::segment_register::DsInfo::Ti>;

  using Index = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::DsInfo>,
      register_info::segment_register::DsInfo::Index>;
};

struct Es : public read_write::ReadWriteRegBase<
                register_info::segment_register::EsInfo> {
  using Rpl = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::EsInfo>,
      register_info::segment_register::EsInfo::Rpl>;

  using Ti = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::EsInfo>,
      register_info::segment_register::EsInfo::Ti>;

  using Index = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::EsInfo>,
      register_info::segment_register::EsInfo::Index>;
};

struct Fs : public read_write::ReadWriteRegBase<
                register_info::segment_register::FsInfo> {
  using Rpl = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::FsInfo>,
      register_info::segment_register::FsInfo::Rpl>;

  using Ti = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::FsInfo>,
      register_info::segment_register::FsInfo::Ti>;

  using Index = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::FsInfo>,
      register_info::segment_register::FsInfo::Index>;
};

struct Gs : public read_write::ReadWriteRegBase<
                register_info::segment_register::GsInfo> {
  using Rpl = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::GsInfo>,
      register_info::segment_register::GsInfo::Rpl>;
  using Ti = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::GsInfo>,
      register_info::segment_register::GsInfo::Ti>;
  using Index = read_write::ReadWriteField<
      read_write::ReadWriteRegBase<register_info::segment_register::GsInfo>,
      register_info::segment_register::GsInfo::Index>;
};
}  // namespace segment_register

}  // namespace regs

}  // namespace detail

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_REGS_HPP_
