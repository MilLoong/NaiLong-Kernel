/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_RISCV64_READ_WRITE_HPP_
#define CPU_IO_INCLUDE_RISCV64_READ_WRITE_HPP_

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
    if constexpr (std::is_same_v<RegInfo, register_info::FpInfo>) {
      __asm__ volatile("mv %0, fp" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::TpInfo>) {
      __asm__ volatile("mv %0, tp" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrr %0, sstatus" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrr %0, stvec" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrr %0, sip" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrr %0, sie" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::TimeInfo>) {
      __asm__ volatile("csrr %0, time" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::CycleInfo>) {
      __asm__ volatile("csrr %0, cycle" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::InstretInfo>) {
      __asm__ volatile("csrr %0, instret" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrr %0, sscratch" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrr %0, sepc" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrr %0, scause" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrr %0, stval" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrr %0, satp" : "=r"(value) : :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StimecmpInfo>) {
      __asm__ volatile("csrr %0, stimecmp" : "=r"(value) : :);
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
    if constexpr (std::is_same_v<RegInfo, register_info::FpInfo>) {
      __asm__ volatile("mv fp, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::TpInfo>) {
      __asm__ volatile("mv tp, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrw sstatus, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrw stvec, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrw sip, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrw sie, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrw sscratch, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrw sepc, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrw scause, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrw stval, %0" : : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrw satp, %0" : : "r"(value) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 写 csr 寄存器，不通过寄存器中转
   * @param value 要写的值
   * @note 只能写 kCsrImmOpMask 范围内的值
   */
  static __always_inline void WriteImm(const uint8_t value) {
    if constexpr (std::is_same_v<RegInfo, register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrwi sstatus, %0" : : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrwi stvec, %0" : : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrwi sip, %0" : : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrwi sie, %0" : : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrwi sscratch, %0" : : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrwi sepc, %0" : : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrwi scause, %0" : : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrwi stval, %0" : : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrwi satp, %0" : : "i"(value) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 通过掩码设置寄存器
   * @param mask 掩码
   */
  static __always_inline void SetBits(uint64_t mask) {
    if constexpr (std::is_same_v<RegInfo, register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrrs zero, sstatus, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrrs zero, stvec, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrrs zero, sip, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrrs zero, sie, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrrs zero, sscratch, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrrs zero, sepc, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrrs zero, scause, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrrs zero, stval, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrrs zero, satp, %0" : : "r"(mask) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 清零寄存器
   * @param mask 掩码
   */
  static __always_inline void ClearBits(uint64_t mask) {
    if constexpr (std::is_same_v<RegInfo, register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrrc zero, sstatus, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrrc zero, stvec, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrrc zero, sip, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrrc zero, sie, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrrc zero, sscratch, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrrc zero, sepc, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrrc zero, scause, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrrc zero, stval, %0" : : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrrc zero, satp, %0" : : "r"(mask) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 通过掩码设置寄存器，不通过寄存器中转
   * @param mask 掩码
   * @note 只能写 kCsrImmOpMask 范围内的值
   */
  static __always_inline void SetBitsImm(const uint8_t mask) {
    if constexpr (std::is_same_v<RegInfo, register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrrsi zero, sstatus, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrrsi zero, stvec, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrrsi zero, sip, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrrsi zero, sie, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrrsi zero, sscratch, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrrsi zero, sepc, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrrsi zero, scause, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrrsi zero, stval, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrrsi zero, satp, %0" : : "i"(mask) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
  }

  /**
   * 清零寄存器，不通过寄存器中转
   * @param mask 掩码
   * @note 只能写 kCsrImmOpMask 范围内的值
   */
  static __always_inline void ClearBitsImm(const uint8_t mask) {
    if constexpr (std::is_same_v<RegInfo, register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrrci zero, sstatus, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrrci zero, stvec, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrrci zero, sip, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrrci zero, sie, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrrci zero, sscratch, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrrci zero, sepc, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrrci zero, scause, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrrci zero, stval, %0" : : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrrci zero, satp, %0" : : "i"(mask) :);
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
    if constexpr ((value & register_info::csr::kCsrImmOpMask) == value) {
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
    if constexpr ((mask & register_info::csr::kCsrImmOpMask) == mask) {
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
    if constexpr ((mask & register_info::csr::kCsrImmOpMask) == mask) {
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
    typename RegInfo::DataType old_value{};
    if constexpr (std::is_same_v<RegInfo, register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrrw %0, sstatus, %1"
                       : "=r"(old_value)
                       : "r"(value)
                       :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrrw %0, stvec, %1" : "=r"(old_value) : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrrw %0, sip, %1" : "=r"(old_value) : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrrw %0, sie, %1" : "=r"(old_value) : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrrw %0, sscratch, %1"
                       : "=r"(old_value)
                       : "r"(value)
                       :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrrw %0, sepc, %1" : "=r"(old_value) : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrrw %0, scause, %1" : "=r"(old_value) : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrrw %0, stval, %1" : "=r"(old_value) : "r"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrrw %0, satp, %1" : "=r"(old_value) : "r"(value) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
    return old_value;
  }

  /**
   * 先读后写寄存器，不通过寄存器中转
   * @param value 要写的值
   * @return RegInfo::DataType 寄存器的值
   * @note 只能写 kCsrImmOpMask 范围内的值
   */
  static __always_inline auto ReadWriteImm(const uint8_t value) ->
      typename RegInfo::DataType {
    typename RegInfo::DataType old_value{};
    if constexpr (std::is_same_v<RegInfo, register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrrwi %0, sstatus, %1"
                       : "=r"(old_value)
                       : "i"(value)
                       :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrrwi %0, stvec, %1" : "=r"(old_value) : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrrwi %0, sip, %1" : "=r"(old_value) : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrrwi %0, sie, %1" : "=r"(old_value) : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrrwi %0, sscratch, %1"
                       : "=r"(old_value)
                       : "i"(value)
                       :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrrwi %0, sepc, %1" : "=r"(old_value) : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrrwi %0, scause, %1"
                       : "=r"(old_value)
                       : "i"(value)
                       :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrrwi %0, stval, %1" : "=r"(old_value) : "i"(value) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrrwi %0, satp, %1" : "=r"(old_value) : "i"(value) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
    return old_value;
  }

  /**
   * 先读后写常数到寄存器
   * @tparam value 要写的值
   * @return RegInfo::DataType 寄存器的值
   */
  template <uint64_t value>
  static __always_inline auto ReadWriteConst() -> typename RegInfo::DataType {
    if constexpr ((value & register_info::csr::kCsrImmOpMask) == value) {
      return ReadWriteRegBase<RegInfo>::ReadWriteImm(value);
    } else {
      return ReadWrite(value);
    }
  }

  /**
   * 先读后通过掩码设置寄存器
   * @param mask 掩码
   * @return RegInfo::DataType 寄存器的值
   */
  static __always_inline auto ReadSetBits(uint64_t mask) ->
      typename RegInfo::DataType {
    typename RegInfo::DataType value{};
    if constexpr (std::is_same_v<RegInfo, register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrrs %0, sstatus, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrrs %0, stvec, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrrs %0, sip, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrrs %0, sie, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrrs %0, sscratch, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrrs %0, sepc, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrrs %0, scause, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrrs %0, stval, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrrs %0, satp, %1" : "=r"(value) : "r"(mask) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
    return value;
  }

  /**
   * 先读后通过掩码设置寄存器，不通过寄存器中转
   * @param mask 掩码
   * @note 只能写 kCsrImmOpMask 范围内的值
   */
  static __always_inline auto ReadSetBitsImm(const uint8_t mask) ->
      typename RegInfo::DataType {
    typename RegInfo::DataType value{};
    if constexpr (std::is_same_v<RegInfo, register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrrsi %0, sstatus, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrrsi %0, stvec, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrrsi %0, sip, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrrsi %0, sie, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrrsi %0, sscratch, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrrsi %0, sepc, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrrsi %0, scause, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrrsi %0, stval, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrrsi %0, satp, %1" : "=r"(value) : "i"(mask) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
    return value;
  }

  /**
   * 通过常数掩码先读后写寄存器
   * @tparam mask 掩码
   * @return RegInfo::DataType 寄存器的值
   */
  template <uint64_t mask>
  static __always_inline auto ReadSetBitsConst() -> typename RegInfo::DataType {
    if constexpr ((mask & register_info::csr::kCsrImmOpMask) == mask) {
      return ReadSetBitsImm(mask);
    } else {
      return ReadSetBits(mask);
    }
  }

  /**
   * 先读后清零寄存器
   * @param mask 掩码
   * @return RegInfo::DataType 寄存器的值
   */
  static __always_inline auto ReadClearBits(uint64_t mask) ->
      typename RegInfo::DataType {
    typename RegInfo::DataType value{};
    if constexpr (std::is_same_v<RegInfo, register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrrc %0, sstatus, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrrc %0, stvec, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrrc %0, sip, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrrc %0, sie, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrrc %0, sscratch, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrrc %0, sepc, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrrc %0, scause, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrrc %0, stval, %1" : "=r"(value) : "r"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrrc %0, satp, %1" : "=r"(value) : "r"(mask) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
    return value;
  }

  /**
   * 先读后清零寄存器，不通过寄存器中转
   * @param mask 掩码
   * @note 只能写 kCsrImmOpMask 范围内的值
   */
  static __always_inline auto ReadClearBitsImm(const uint8_t mask) ->
      typename RegInfo::DataType {
    typename RegInfo::DataType value{};
    if constexpr (std::is_same_v<RegInfo, register_info::csr::SstatusInfo>) {
      __asm__ volatile("csrrci %0, sstatus, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvecInfo>) {
      __asm__ volatile("csrrci %0, stvec, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SipInfo>) {
      __asm__ volatile("csrrci %0, sip, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo, register_info::csr::SieInfo>) {
      __asm__ volatile("csrrci %0, sie, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SscratchInfo>) {
      __asm__ volatile("csrrci %0, sscratch, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SepcInfo>) {
      __asm__ volatile("csrrci %0, sepc, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::ScauseInfo>) {
      __asm__ volatile("csrrci %0, scause, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::StvalInfo>) {
      __asm__ volatile("csrrci %0, stval, %1" : "=r"(value) : "i"(mask) :);
    } else if constexpr (std::is_same_v<RegInfo,
                                        register_info::csr::SatpInfo>) {
      __asm__ volatile("csrrci %0, satp, %1" : "=r"(value) : "i"(mask) :);
    } else {
      static_assert(sizeof(RegInfo) == 0);
    }
    return value;
  }

  /**
   * 通过常数掩码先读后清零寄存器
   * @tparam mask 掩码
   * @return RegInfo::DataType 寄存器的值
   */
  template <uint64_t mask>
  static __always_inline auto ReadClearBitsConst() ->
      typename RegInfo::DataType {
    if constexpr ((mask & register_info::csr::kCsrImmOpMask) == mask) {
      return WriteOnlyRegBase<RegInfo>::ReadClearBitsImm(mask);
    } else {
      return ReadClearBits(mask);
    }
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
  static __always_inline void Set() {
    if constexpr ((RegInfo::kBitMask & register_info::csr::kCsrImmOpMask) ==
                  RegInfo::kBitMask) {
      Reg::SetBitsImm(RegInfo::kBitMask);
    } else {
      Reg::SetBits(RegInfo::kBitMask);
    }
  }

  /**
   * 清零对应 Reg 的由 RegInfo 规定的指定位
   */
  static __always_inline void Clear() {
    if constexpr ((RegInfo::kBitMask & register_info::csr::kCsrImmOpMask) ==
                  RegInfo::kBitMask) {
      Reg::ClearBitsImm(RegInfo::kBitMask);
    } else {
      Reg::ClearBits(RegInfo::kBitMask);
    }
  }
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
    auto new_value =
        (org_value & ~RegInfo::kBitMask) |
        ((static_cast<decltype(org_value)>(value) << RegInfo::kBitOffset) &
         RegInfo::kBitMask);
    Reg::Write(new_value);
    return static_cast<typename RegInfo::DataType>(
        (org_value & RegInfo::kBitMask) >> RegInfo::kBitOffset);
  }
};

}  // namespace read_write

}  // namespace detail

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_RISCV64_READ_WRITE_HPP_
