/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_RISCV64_REGISTER_INFO_HPP_
#define CPU_IO_INCLUDE_RISCV64_REGISTER_INFO_HPP_

#include <array>
#include <cstdint>

#include "register_info_base.h"

namespace cpu_io {

namespace detail {

namespace register_info {

/// 通用寄存器
struct FpInfo : public RegInfoBase {};
struct TpInfo : public RegInfoBase {};

namespace csr {
/// 立即数掩码，大于这个值需要使用寄存器中转
static constexpr uint64_t kCsrImmOpMask = 0x1F;

/**
 * @brief sstatus 寄存器定义
 * @see priv-isa.pdf#10.1.1
 */
struct SstatusInfo : public RegInfoBase {
  struct Sie {
    using DataType = bool;
    static constexpr uint64_t kBitOffset = 1;
    static constexpr uint64_t kBitWidth = 1;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };

  struct Spie {
    using DataType = bool;
    static constexpr uint64_t kBitOffset = 5;
    static constexpr uint64_t kBitWidth = 1;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };

  struct Spp {
    using DataType = bool;
    static constexpr uint64_t kBitOffset = 8;
    static constexpr uint64_t kBitWidth = 1;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };
};

/**
 * @brief stvec 寄存器定义
 * @see priv-isa.pdf#10.1.2
 */
struct StvecInfo : public RegInfoBase {
  /// 中断模式 直接
  static constexpr uint64_t kDirect = 0x0;
  /// 中断模式 向量
  static constexpr uint64_t kVectored = 0x1;

  struct Mode {
    using DataType = uint8_t;
    static constexpr uint64_t kBitOffset = 0;
    static constexpr uint64_t kBitWidth = 2;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };

  struct Base {
    using DataType = uint64_t;
    static constexpr uint64_t kBitOffset = 0;
    static constexpr uint64_t kBitWidth = 62;
    static constexpr uint64_t kBitMask = ~0x3;
    static constexpr uint64_t kAllSetMask = ~0x3;
  };
};

/**
 * @brief sip 寄存器定义
 * @see priv-isa.pdf#10.1.3
 */
struct SipInfo : public RegInfoBase {
  struct Ssip {
    using DataType = bool;
    static constexpr uint64_t kBitOffset = 1;
    static constexpr uint64_t kBitWidth = 1;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };

  struct Stip {
    using DataType = bool;
    static constexpr uint64_t kBitOffset = 5;
    static constexpr uint64_t kBitWidth = 1;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };

  struct Seip {
    using DataType = bool;
    static constexpr uint64_t kBitOffset = 9;
    static constexpr uint64_t kBitWidth = 1;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };
};

/**
 * @brief sip 寄存器定义
 * @see priv-isa.pdf#10.1.3
 */
struct SieInfo : public RegInfoBase {
  struct Ssie {
    using DataType = bool;
    static constexpr uint64_t kBitOffset = 1;
    static constexpr uint64_t kBitWidth = 1;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };

  struct Stie {
    using DataType = bool;
    static constexpr uint64_t kBitOffset = 5;
    static constexpr uint64_t kBitWidth = 1;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };

  struct Seie {
    using DataType = bool;
    static constexpr uint64_t kBitOffset = 9;
    static constexpr uint64_t kBitWidth = 1;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };
};

/**
 * @brief time 寄存器定义
 * @see priv-isa.pdf#10.1.4
 */
struct TimeInfo : public RegInfoBase {};

/**
 * @brief cycle 寄存器定义
 * @see priv-isa.pdf#10.1.4
 */
struct CycleInfo : public RegInfoBase {};

/**
 * @brief instret 寄存器定义
 * @see priv-isa.pdf#10.1.4
 */
struct InstretInfo : public RegInfoBase {};

/**
 * @brief sscratch 寄存器定义
 * @see priv-isa.pdf#10.1.6
 */
struct SscratchInfo : public RegInfoBase {};

/**
 * @brief sepc 寄存器定义
 * @see priv-isa.pdf#10.1.7
 */
struct SepcInfo : public RegInfoBase {};

/**
 * @brief scause 寄存器定义
 * @see priv-isa.pdf#10.1.8
 */
struct ScauseInfo : public RegInfoBase {
  enum {
    // 中断
    kInterrupt = 1ULL << 63,
    kSupervisorSoftwareInterrupt = kInterrupt + 1,
    kSupervisorTimerInterrupt = kInterrupt + 5,
    kSupervisorExternalInterrupt = kInterrupt + 9,
    kCounterOverflowInterrupt = kInterrupt + 13,

    // 异常
    kInstructionAddressMisaligned = 0,
    kInstructionAccessFault = 1,
    kIllegalInstruction = 2,
    kBreakpoint = 3,
    kLoadAddressMisaligned = 4,
    kLoadAccessFault = 5,
    kStoreAmoAddressMisaligned = 6,
    kStoreAmoAccessFault = 7,
    kEcallUserMode = 8,
    kEcallSuperMode = 9,
    kReserved4 = 10,
    kEcallMachineMode = 11,
    kInstructionPageFault = 12,
    kLoadPageFault = 13,
    kReserved5 = 14,
    kStoreAmoPageFault = 15,
    kSoftwareCheck = 18,
    kHardwareError = 19,
  };

  /// 最大中断数
  static constexpr uint32_t kInterruptMaxCount = 16;

  /// 中断名
  static constexpr std::array<const char *, kInterruptMaxCount>
      kInterruptNames = {
          "Reserved", "Supervisor Software Interrupt", "Reserved", "Reserved",
          "Reserved", "Supervisor Timer Interrupt",    "Reserved", "Reserved",
          "Reserved", "Supervisor External Interrupt", "Reserved", "Reserved",
          "Reserved", "Counter-overflow Interrupt",    "Reserved", "Reserved",
      };

  /// 最大异常数
  static constexpr uint32_t kExceptionMaxCount = 20;

  /// 异常名
  static constexpr std::array<const char *, kExceptionMaxCount>
      kExceptionNames = {
          "Instruction Address Misaligned",
          "Instruction Access Fault",
          "Illegal Instruction",
          "Breakpoint",
          "Load Address Misaligned",
          "Load Access Fault",
          "Store/AMO Address Misaligned",
          "Store/AMO Access Fault",
          "Environment Call from U-mode",
          "Environment Call from S-mode",
          "Reserved",
          "Reserved",
          "Instruction Page Fault",
          "Load Page Fault",
          "Reserved",
          "Store/AMO Page Fault",
          "Reserved",
          "Reserved",
          "SoftwareCheck",
          "HardwareError",
      };

  struct ExceptionCode {
    using DataType = uint64_t;
    static constexpr uint64_t kBitOffset = 0;
    static constexpr uint64_t kBitWidth = 63;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };

  struct Interrupt {
    using DataType = bool;
    static constexpr uint64_t kBitOffset = 63;
    static constexpr uint64_t kBitWidth = 1;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };
};

/**
 * @brief stval 寄存器定义
 * @see priv-isa.pdf#10.1.9
 */
struct StvalInfo : public RegInfoBase {};

/**
 * @brief satp 寄存器定义
 * @see priv-isa.pdf#10.1.11
 */
struct SatpInfo : public RegInfoBase {
  enum : uint8_t {
    kBare = 0,
    kSv39 = 8,
    kSv48 = 9,
    kSv57 = 10,
    kSv64 = 11,
  };

  /// 最大内存模式数
  static constexpr uint32_t kModeMaxCount = 16;

  static constexpr std::array<const char *, kModeMaxCount> kModeNames = {
      "Bare",     "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
      "Reserved", "Reserved", "SV39",     "SV48",     "SV57",     "SV64",
  };

  static constexpr uint64_t kPpnOffset = 12;

  struct Ppn {
    using DataType = uint64_t;
    static constexpr uint64_t kBitOffset = 0;
    static constexpr uint64_t kBitWidth = 44;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };
  struct Asid {
    using DataType = uint16_t;
    static constexpr uint64_t kBitOffset = 44;
    static constexpr uint64_t kBitWidth = 16;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };
  struct Mode {
    using DataType = uint8_t;
    static constexpr uint64_t kBitOffset = 60;
    static constexpr uint64_t kBitWidth = 4;
    static constexpr uint64_t kBitMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) << kBitOffset : ~0ULL;
    static constexpr uint64_t kAllSetMask =
        (kBitWidth < 64) ? ((1ULL << kBitWidth) - 1) : ~0ULL;
  };
};

/**
 * @brief stimecmp 寄存器定义
 * @see priv-isa.pdf#16.1.1
 */
struct StimecmpInfo : public RegInfoBase {};

}  // namespace csr

}  // namespace register_info

}  // namespace detail

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_RISCV64_REGISTER_INFO_HPP_
