/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_X86_64_READ_WRITE_HPP_
#define CPU_IO_INCLUDE_X86_64_READ_WRITE_HPP_

#include <cstdint>

#include "register_info.hpp"

namespace cpu_io {

namespace detail {

namespace read_write {
/**
 * 只读接口
 * @tparam 寄存器类型
 */
template <class RegInfo>
class ReadOnlyRegBase {
 public:
  /// @name 构造/析构函数
  /// @{
  ReadOnlyRegBase() = default;
  ReadOnlyRegBase(const ReadOnlyRegBase &) = delete;
  ReadOnlyRegBase(ReadOnlyRegBase &&) = delete;
  auto operator=(const ReadOnlyRegBase &) -> ReadOnlyRegBase & = delete;
  auto operator=(ReadOnlyRegBase &&) -> ReadOnlyRegBase & = delete;
  ~ReadOnlyRegBase() = default;
  /// @}

  /**
   * 读寄存器
   * @return RegInfo::DataType 寄存器的值
   */
  static __always_inline auto Read() -> typename RegInfo::DataType {
    typename RegInfo::DataType value{};
    if constexpr (std::is_same_v<RegInfo, register_info::RbpInfo>) {
      __asm__ volatile("mov %%rbp, %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::RflagsInfo>) {
      __asm__ volatile("pushfq; popq %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::GdtrInfo>) {
      __asm__ volatile("sgdt %0" : "=m"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::LdtrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::IdtrInfo>) {
      __asm__ volatile("sidt %0" : "=m"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::TrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr0Info>) {
      __asm__ volatile("mov %%cr0, %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr2Info>) {
      __asm__ volatile("mov %%cr2, %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr3Info>) {
      __asm__ volatile("mov %%cr3, %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr4Info>) {
      __asm__ volatile("mov %%cr4, %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr8Info>) {
      __asm__ volatile("mov %%cr8, %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::Xcr0Info>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::CsInfo>) {
      __asm__ volatile("mov %%cs, %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::SsInfo>) {
      __asm__ volatile("mov %%ss, %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::DsInfo>) {
      __asm__ volatile("mov %%ds, %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::EsInfo>) {
      __asm__ volatile("mov %%es, %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::FsInfo>) {
      __asm__ volatile("mov %%fs, %0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::GsInfo>) {
      __asm__ volatile("mov %%gs, %0" : "=r"(value) : :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
    return value;
  }

  static __always_inline auto Read(uint32_t offset) ->
      typename RegInfo::DataType {
    typename RegInfo::DataType value{};
    if constexpr (std::is_same_v<RegInfo, register_info::MsrInfo>) {
      uint32_t low{};
      uint32_t high{};
      __asm__ volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(offset) :);
      value = (static_cast<uint64_t>(high) << 32) | low;
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
    return value;
  }

  /**
   * () 重载
   */
  __always_inline auto operator()() -> typename RegInfo::DataType {
    return Read();
  }
};

/**
 * 只写接口
 * @tparam 寄存器类型
 */
template <class RegInfo>
class WriteOnlyRegBase {
 public:
  /// @name 构造/析构函数
  /// @{
  WriteOnlyRegBase() = default;
  WriteOnlyRegBase(const WriteOnlyRegBase &) = delete;
  WriteOnlyRegBase(WriteOnlyRegBase &&) = delete;
  auto operator=(const WriteOnlyRegBase &) -> WriteOnlyRegBase & = delete;
  auto operator=(WriteOnlyRegBase &&) -> WriteOnlyRegBase & = delete;
  ~WriteOnlyRegBase() = default;
  /// @}

  /**
   * 写寄存器
   * @param value 要写的值
   */
  static __always_inline void Write(typename RegInfo::DataType value) {
    if constexpr (std::is_same_v<RegInfo, register_info::RbpInfo>) {
      __asm__ volatile("mov %0, %%rbp" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::RflagsInfo>) {
      __asm__ volatile("pushq %0; popfq" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::GdtrInfo>) {
      __asm__ volatile("lgdt %0" : : "m"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::LdtrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::IdtrInfo>) {
      __asm__ volatile("lidt %0" : : "m"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::TrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr0Info>) {
      __asm__ volatile("mov %0, %%cr0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr2Info>) {
      __asm__ volatile("mov %0, %%cr2" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr3Info>) {
      __asm__ volatile("mov %0, %%cr3" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr4Info>) {
      __asm__ volatile("mov %0, %%cr4" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr8Info>) {
      __asm__ volatile("mov %0, %%cr8" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::Xcr0Info>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::CsInfo>) {
      auto JumpFunction = [=](uint16_t value) {
        __asm__ volatile(
            "push %0\n\t"
            "pushq $WriteCsLabel\n\t"
            "retfq\n\t"
            "WriteCsLabel: \n\t"
            :
            : "r"(value)
            :);
      };
      JumpFunction(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::SsInfo>) {
      __asm__ volatile("mov %0, %%ss" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::DsInfo>) {
      __asm__ volatile("mov %0, %%ds" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::EsInfo>) {
      __asm__ volatile("mov %0, %%es" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::FsInfo>) {
      __asm__ volatile("mov %0, %%fs" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::segment_register::GsInfo>) {
      __asm__ volatile("mov %0, %%gs" : : "r"(value) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  static __always_inline void Write(uint32_t offset,
                                    typename RegInfo::DataType value) {
    if constexpr (std::is_same_v<RegInfo, register_info::MsrInfo>) {
      uint32_t low = value & 0xFFFFFFFF;
      uint32_t high = value >> 32;
      __asm__ volatile("wrmsr" : : "c"(offset), "a"(low), "d"(high) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 通过偏移设置寄存器
   * @param offset 位偏移
   */
  static __always_inline void SetBits(uint64_t offset) {
    if constexpr (std::is_same_v<RegInfo, register_info::RbpInfo>) {
      __asm__ volatile("bts %%rbp, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::RflagsInfo>) {
      if (offset == register_info::RflagsInfo::If::kBitOffset) {
        __asm__ volatile("sti");
      } else {
        typename RegInfo::DataType old_value = 0;
        __asm__ volatile("pushfq; popq %0" : "=r"(old_value) : :);
        auto new_value = old_value | (1 << offset);
        Write(new_value);
      }
    } else if constexpr (std::is_same_v<RegInfo, register_info::GdtrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::LdtrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::IdtrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::TrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr0Info>) {
      __asm__ volatile("bts %%cr0, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr2Info>) {
      __asm__ volatile("bts %%cr2, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr3Info>) {
      __asm__ volatile("bts %%cr3, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr4Info>) {
      __asm__ volatile("bts %%cr4, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr8Info>) {
      __asm__ volatile("bts %%cr8, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::Xcr0Info>) {
      static_assert(sizeof(RegInfo) == 0);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 清零寄存器
   * @param offset 位偏移
   */
  static __always_inline void ClearBits(uint64_t offset) {
    if constexpr (std::is_same_v<RegInfo, register_info::RbpInfo>) {
      __asm__ volatile("btr %%rbp, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::RflagsInfo>) {
      if (offset == register_info::RflagsInfo::If::kBitOffset) {
        __asm__ volatile("cli");
      } else {
        typename RegInfo::DataType old_value = 0;
        __asm__ volatile("pushfq; popq %0" : "=r"(old_value) : :);
        auto new_value = old_value & (~(1ULL << offset));
        Write(new_value);
      }
    } else if constexpr (std::is_same_v<RegInfo, register_info::GdtrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::LdtrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::IdtrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::TrInfo>) {
      static_assert(sizeof(RegInfo) == 0);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr0Info>) {
      __asm__ volatile("btr %%cr0, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr2Info>) {
      __asm__ volatile("btr %%cr2, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr3Info>) {
      __asm__ volatile("btr %%cr3, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr4Info>) {
      __asm__ volatile("btr %%cr4, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::cr::Cr8Info>) {
      __asm__ volatile("btr %%cr8, %0" : : "r"(offset) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::Xcr0Info>) {
      static_assert(sizeof(RegInfo) == 0);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * |= 重载
   */
  __always_inline void operator|=(uint64_t offset) { SetBits(offset); }
};

/**
 * 读写接口
 * @tparam 寄存器类型
 */
template <class RegInfo>
class ReadWriteRegBase : public ReadOnlyRegBase<RegInfo>,
                         public WriteOnlyRegBase<RegInfo> {
 public:
  /// @name 构造/析构函数
  /// @{
  ReadWriteRegBase() = default;
  ReadWriteRegBase(const ReadWriteRegBase &) = delete;
  ReadWriteRegBase(ReadWriteRegBase &&) = delete;
  auto operator=(const ReadWriteRegBase &) -> ReadWriteRegBase & = delete;
  auto operator=(ReadWriteRegBase &&) -> ReadWriteRegBase & = delete;
  ~ReadWriteRegBase() = default;
  /// @}

  /**
   * 先读后写寄存器
   * @tparam value 要写的值
   * @return RegInfo::DataType 寄存器的值
   */
  static __always_inline auto ReadWrite(typename RegInfo::DataType value) ->
      typename RegInfo::DataType {
    auto old_value = ReadOnlyRegBase<RegInfo>::Read();
    WriteOnlyRegBase<RegInfo>::Write(value);
    return old_value;
  }

  /**
   * 通过偏移先读后写寄存器
   * @param offset 偏移
   * @return RegInfo::DataType 寄存器的值
   */
  static __always_inline auto ReadSetBits(uint64_t offset) ->
      typename RegInfo::DataType {
    auto old_value = ReadOnlyRegBase<RegInfo>::Read();
    WriteOnlyRegBase<RegInfo>::SetBits(offset);
    return old_value;
  }

  /**
   * 通过偏移先读后清零寄存器
   * @param offset 偏移
   * @return RegInfo::DataType 寄存器的值
   */
  static __always_inline auto ReadClearBits(uint64_t offset) ->
      typename RegInfo::DataType {
    auto old_value = ReadOnlyRegBase<RegInfo>::Read();
    WriteOnlyRegBase<RegInfo>::ClearBits(offset);
    return old_value;
  }
};

/**
 * 只读位域接口
 * @tparam Reg 寄存器类型
 * @tparam RegInfo 寄存器数据信息
 */
template <class Reg, class RegInfo>
class ReadOnlyField {
 public:
  /// @name 构造/析构函数
  /// @{
  ReadOnlyField() = default;
  ReadOnlyField(const ReadOnlyField &) = delete;
  ReadOnlyField(ReadOnlyField &&) = delete;
  auto operator=(const ReadOnlyField &) -> ReadOnlyField & = delete;
  auto operator=(ReadOnlyField &&) -> ReadOnlyField & = delete;
  ~ReadOnlyField() = default;
  /// @}

  /**
   * 获取对应 Reg 的由 RegInfo 规定的指定位的值
   * @return RegInfo::DataType 指定位值的信息
   */
  static __always_inline auto Get() -> typename RegInfo::DataType {
    if constexpr (std::is_same_v<RegInfo, register_info::GdtrInfo::Limit>) {
      return Reg::Read().limit;
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::GdtrInfo::Base>) {
      return Reg::Read().base;
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::IdtrInfo::Limit>) {
      return Reg::Read().limit;
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::IdtrInfo::Base>) {
      return Reg::Read().base;
    } else {
      return static_cast<typename RegInfo::DataType>(
          (Reg::Read() & RegInfo::kBitMask) >> RegInfo::kBitOffset);
    }
  }

  /**
   * 从指定的值获取对应 Reg 的由 RegInfo 规定的指定位的值
   * @param value 指定的值
   * @return RegInfo::DataType 指定位值的信息
   */
  static __always_inline auto Get(uint64_t value) ->
      typename RegInfo::DataType {
    if constexpr (std::is_same_v<RegInfo, register_info::GdtrInfo::Limit>) {
      return value;
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::GdtrInfo::Base>) {
      return value;
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::IdtrInfo::Limit>) {
      return value;
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::IdtrInfo::Base>) {
      return value;
    } else {
      return static_cast<typename RegInfo::DataType>(
          (value & RegInfo::kBitMask) >> RegInfo::kBitOffset);
    }
  }
};

/**
 * 只写位域接口
 * @tparam Reg 寄存器类型
 * @tparam RegInfo 寄存器数据信息
 */
template <class Reg, class RegInfo>
class WriteOnlyField {
 public:
  /// @name 构造/析构函数
  /// @{
  WriteOnlyField() = default;
  WriteOnlyField(const WriteOnlyField &) = delete;
  WriteOnlyField(WriteOnlyField &&) = delete;
  auto operator=(const WriteOnlyField &) -> WriteOnlyField & = delete;
  auto operator=(WriteOnlyField &&) -> WriteOnlyField & = delete;
  ~WriteOnlyField() = default;
  /// @}

  /**
   * 置位对应 Reg 的由 RegInfo 规定的指定位
   */
  static __always_inline void Set() { Reg::SetBits(RegInfo::kBitOffset); }

  /**
   * 清零对应 Reg 的由 RegInfo 规定的指定位
   */
  static __always_inline void Clear() { Reg::ClearBits(RegInfo::kBitOffset); }
};

/**
 * 读写位域接口
 * @tparam Reg 寄存器类型
 * @tparam RegInfo 寄存器数据信息
 */
template <class Reg, class RegInfo>
class ReadWriteField : public ReadOnlyField<Reg, RegInfo>,
                       public WriteOnlyField<Reg, RegInfo> {
 public:
  /// @name 构造/析构函数
  /// @{
  ReadWriteField() = default;
  ReadWriteField(const ReadWriteField &) = delete;
  ReadWriteField(ReadWriteField &&) = delete;
  auto operator=(const ReadWriteField &) -> ReadWriteField & = delete;
  auto operator=(ReadWriteField &&) -> ReadWriteField & = delete;
  ~ReadWriteField() = default;
  /// @}

  /**
   * 将寄存器的原值替换为指定值
   * @param value 新值
   */
  static __always_inline void Write(typename RegInfo::DataType value) {
    auto org_value = Reg::Read();
    auto new_value = (org_value & ~RegInfo::kBitMask) |
                     ((value << RegInfo::kBitOffset) & RegInfo::kBitMask);
    Reg::Write(new_value);
  }

  /**
   * 先读出旧值，后将寄存器的值替换为指定值
   * @param value 新值
   * @return RegInfo::DataType 由寄存器规定的数据类型
   */
  static __always_inline auto ReadWrite(typename RegInfo::DataType value) ->
      typename RegInfo::DataType {
    auto org_value = Reg::Read();
    auto new_value = (org_value & ~RegInfo::kBitMask) |
                     ((value << RegInfo::kBitOffset) & RegInfo::kBitMask);
    Reg::Write(new_value);
    return static_cast<typename RegInfo::DataType>(
        (org_value & RegInfo::kBitMask) >> RegInfo::kBitOffset);
  }
};

}  // namespace read_write

}  // namespace detail

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_READ_WRITE_HPP_
