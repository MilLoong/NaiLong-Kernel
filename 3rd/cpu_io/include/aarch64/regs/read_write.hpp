/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_AARCH64_READ_WRITE_HPP_
#define CPU_IO_INCLUDE_AARCH64_READ_WRITE_HPP_

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
    if constexpr (std::is_same_v<RegInfo, register_info::X0Info>) {
      __asm__ volatile("mov %0, x0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::X29Info>) {
      __asm__ volatile("mov %0, x29" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CPACR_EL1Info>) {
      __asm__ volatile("mrs %0, CPACR_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CurrentELInfo>) {
      __asm__ volatile("mrs %0, CurrentEL" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::system_reg::SPSelInfo>) {
      __asm__ volatile("mrs %0, SPSel" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::system_reg::DAIFInfo>) {
      __asm__ volatile("mrs %0, DAIF" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::VBAR_EL1Info>) {
      __asm__ volatile("mrs %0, VBAR_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::ELR_EL1Info>) {
      __asm__ volatile("mrs %0, ELR_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::SPSR_EL1Info>) {
      __asm__ volatile("mrs %0, SPSR_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::SP_EL0Info>) {
      __asm__ volatile("mrs %0, SP_EL0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::SP_EL1Info>) {
      __asm__ volatile("mrs %0, SP_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::MPIDR_EL1Info>) {
      __asm__ volatile("mrs %0, MPIDR_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::SCTLR_EL1Info>) {
      __asm__ volatile("mrs %0, SCTLR_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::MAIR_EL1Info>) {
      __asm__ volatile("mrs %0, MAIR_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::TCR_EL1Info>) {
      __asm__ volatile("mrs %0, TCR_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::TTBR0_EL1Info>) {
      __asm__ volatile("mrs %0, TTBR0_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::TTBR1_EL1Info>) {
      __asm__ volatile("mrs %0, TTBR1_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::ESR_EL1Info>) {
      __asm__ volatile("mrs %0, ESR_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::FAR_EL1Info>) {
      __asm__ volatile("mrs %0, FAR_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CNTV_CTL_EL0Info>) {
      __asm__ volatile("mrs %0, CNTV_CTL_EL0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CNTV_TVAL_EL0Info>) {
      __asm__ volatile("mrs %0, CNTV_TVAL_EL0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CNTVCT_EL0Info>) {
      __asm__ volatile("mrs %0, CNTVCT_EL0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CNTFRQ_EL0Info>) {
      __asm__ volatile("mrs %0, CNTFRQ_EL0" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_PMR_EL1Info>) {
      __asm__ volatile("mrs %0, ICC_PMR_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_IGRPEN1_EL1Info>) {
      __asm__ volatile("mrs %0, ICC_IGRPEN1_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_SRE_EL1Info>) {
      __asm__ volatile("mrs %0, ICC_SRE_EL1" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_IAR1_EL1Info>) {
      __asm__ volatile("mrs %0, ICC_IAR1_EL1" : "=r"(value) : :);
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
    if constexpr (std::is_same_v<RegInfo, register_info::X0Info>) {
      __asm__ volatile("mov x0, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::X29Info>) {
      __asm__ volatile("mov x29, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CPACR_EL1Info>) {
      __asm__ volatile("msr CPACR_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::system_reg::SPSelInfo>) {
      __asm__ volatile("msr SPSel, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::system_reg::DAIFInfo>) {
      __asm__ volatile("msr DAIF, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::VBAR_EL1Info>) {
      __asm__ volatile("msr VBAR_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::ELR_EL1Info>) {
      __asm__ volatile("msr ELR_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::SPSR_EL1Info>) {
      __asm__ volatile("msr SPSR_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::SP_EL0Info>) {
      __asm__ volatile("msr SP_EL0, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::SP_EL1Info>) {
      __asm__ volatile("msr SP_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::SCTLR_EL1Info>) {
      __asm__ volatile("msr SCTLR_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::MAIR_EL1Info>) {
      __asm__ volatile("msr MAIR_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::TCR_EL1Info>) {
      __asm__ volatile("msr TCR_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::TTBR0_EL1Info>) {
      __asm__ volatile("msr TTBR0_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::TTBR1_EL1Info>) {
      __asm__ volatile("msr TTBR1_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::ESR_EL1Info>) {
      __asm__ volatile("msr ESR_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo, register_info::system_reg::FAR_EL1Info>) {
      __asm__ volatile("msr FAR_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CNTV_CTL_EL0Info>) {
      __asm__ volatile("msr CNTV_CTL_EL0, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CNTV_TVAL_EL0Info>) {
      __asm__ volatile("msr CNTV_TVAL_EL0, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CNTFRQ_EL0Info>) {
      __asm__ volatile("msr CNTFRQ_EL0, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_PMR_EL1Info>) {
      __asm__ volatile("msr ICC_PMR_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_IGRPEN1_EL1Info>) {
      __asm__ volatile("msr ICC_IGRPEN1_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_SRE_EL1Info>) {
      __asm__ volatile("msr ICC_SRE_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_EOIR1_EL1Info>) {
      __asm__ volatile("msr ICC_EOIR1_EL1, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_SGI1R_EL1Info>) {
      __asm__ volatile("msr ICC_SGI1R_EL1, %0" : : "r"(value) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 写 PSTATE 寄存器，不通过寄存器中转
   * @param value 要写的值
   * @note 只能写 kPSTATEImmOpMask 范围内的值
   */
  static __always_inline void WriteImm(const uint8_t value) {
    if constexpr (std::is_same_v<RegInfo,
                                 register_info::system_reg::SPSelInfo>) {
      __asm__ volatile("msr SPSel, %0" : : "i"(value) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 通过掩码设置寄存器
   * @param mask 掩码
   */
  static __always_inline void SetBits(uint64_t mask) {
    if constexpr (std::is_same_v<RegInfo,
                                 register_info::system_reg::CPACR_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, CPACR_EL1" : "=r"(value)::);
      value |= mask;
      Write(value);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::system_reg::SPSelInfo>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, SPSel" : "=r"(value)::);
      value |= mask;
      Write(value);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::system_reg::DAIFInfo>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, DAIF" : "=r"(value)::);
      value |= mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CNTV_CTL_EL0Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, CNTV_CTL_EL0" : "=r"(value)::);
      value |= mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_PMR_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, ICC_PMR_EL1" : "=r"(value)::);
      value |= mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_SRE_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, ICC_SRE_EL1" : "=r"(value)::);
      value |= mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_IGRPEN1_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, ICC_IGRPEN1_EL1" : "=r"(value)::);
      value |= mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::SCTLR_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, SCTLR_EL1" : "=r"(value)::);
      value |= mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::TTBR0_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, TTBR0_EL1" : "=r"(value)::);
      value |= mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::TTBR1_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, TTBR1_EL1" : "=r"(value)::);
      value |= mask;
      Write(value);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 清零寄存器
   * @param mask 掩码
   */
  static __always_inline void ClearBits(uint64_t mask) {
    if constexpr (std::is_same_v<RegInfo,
                                 register_info::system_reg::CPACR_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, CPACR_EL1" : "=r"(value)::);
      value &= ~mask;
      Write(value);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::system_reg::SPSelInfo>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, SPSel" : "=r"(value)::);
      value &= ~mask;
      Write(value);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::system_reg::DAIFInfo>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, DAIF" : "=r"(value)::);
      value &= ~mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::CNTV_CTL_EL0Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, CNTV_CTL_EL0" : "=r"(value)::);
      value &= ~mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::ICC_IGRPEN1_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, ICC_IGRPEN1_EL1" : "=r"(value)::);
      value &= ~mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::SCTLR_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, SCTLR_EL1" : "=r"(value)::);
      value &= ~mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::TTBR0_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, TTBR0_EL1" : "=r"(value)::);
      value &= ~mask;
      Write(value);
    } else if constexpr (std::is_same_v<
                             RegInfo,
                             register_info::system_reg::TTBR1_EL1Info>) {
      typename RegInfo::DataType value = 0;
      __asm__ volatile("mrs %0, TTBR1_EL1" : "=r"(value)::);
      value &= ~mask;
      Write(value);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 通过掩码设置寄存器，不通过寄存器中转
   * @param mask 掩码
   * @note 只能写 kPSTATEImmOpMask 范围内的值
   */
  static __always_inline void SetBitsImm(const uint8_t mask) {
    if constexpr (std::is_same_v<RegInfo,
                                 register_info::system_reg::DAIFInfo>) {
      __asm__ volatile("msr DAIFSet, %0" : : "i"(mask) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 清零寄存器，不通过寄存器中转
   * @param mask 掩码
   * @note 只能写 kPSTATEImmOpMask 范围内的值
   */
  static __always_inline void ClearBitsImm(const uint8_t mask) {
    if constexpr (std::is_same_v<RegInfo,
                                 register_info::system_reg::DAIFInfo>) {
      __asm__ volatile("msr DAIFClr, %0" : : "i"(mask) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 向寄存器写常数
   * @tparam value 常数的值
   */
  template <uint64_t value>
  static __always_inline void WriteConst() {
    if constexpr ((value & register_info::system_reg::kPSTATEImmOpMask) ==
                  value) {
      WriteImm(value);
    } else {
      Write(value);
    }
  }

  /**
   * 通过掩码写寄存器
   * @tparam mask 掩码
   */
  template <uint64_t mask>
  static __always_inline void SetConst() {
    if constexpr ((mask & register_info::system_reg::kPSTATEImmOpMask) ==
                  mask) {
      SetBitsImm(mask);
    } else {
      SetBits(mask);
    }
  }

  /**
   * 通过掩码清零寄存器
   * @tparam mask 掩码
   */
  template <uint64_t mask>
  static __always_inline void ClearConst() {
    if constexpr ((mask & register_info::system_reg::kPSTATEImmOpMask) ==
                  mask) {
      ClearBitsImm(mask);
    } else {
      ClearBits(mask);
    }
  }

  /**
   * |= 重载
   */
  __always_inline void operator|=(uint64_t mask) { SetBits(mask); }
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
   * 先读后通过掩码设置寄存器
   * @param mask 掩码
   * @return RegInfo::DataType 寄存器的值
   */
  static __always_inline auto ReadSetBits(uint64_t mask) ->
      typename RegInfo::DataType {
    auto old_value = ReadOnlyRegBase<RegInfo>::Read();
    WriteOnlyRegBase<RegInfo>::SetBits(mask);
    return old_value;
  }

  /**
   * 先读后清零寄存器
   * @param mask 掩码
   * @return RegInfo::DataType 寄存器的值
   */
  static __always_inline auto ReadClearBits(uint64_t mask) ->
      typename RegInfo::DataType {
    auto old_value = ReadOnlyRegBase<RegInfo>::Read();
    WriteOnlyRegBase<RegInfo>::ClearBits(mask);
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
    return static_cast<typename RegInfo::DataType>(
        (Reg::Read() & RegInfo::kBitMask) >> RegInfo::kBitOffset);
  }

  /**
   * 从指定的值获取对应 Reg 的由 RegInfo 规定的指定位的值
   * @param value 指定的值
   * @return RegInfo::DataType 指定位值的信息
   */
  static __always_inline auto Get(uint64_t value) ->
      typename RegInfo::DataType {
    return static_cast<typename RegInfo::DataType>(
        (value & RegInfo::kBitMask) >> RegInfo::kBitOffset);
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
  static __always_inline void Set() { Reg::SetBits(RegInfo::kBitMask); }

  /**
   * 清零对应 Reg 的由 RegInfo 规定的指定位
   */
  static __always_inline void Clear() { Reg::ClearBits(RegInfo::kBitMask); }
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
    auto new_value =
        (org_value & ~RegInfo::kBitMask) |
        ((static_cast<decltype(org_value)>(value) << RegInfo::kBitOffset) &
         RegInfo::kBitMask);
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

#endif  // CPU_IO_INCLUDE_AARCH64_READ_WRITE_HPP_
