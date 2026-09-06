/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_X86_64_PIT_HPP_
#define CPU_IO_INCLUDE_X86_64_PIT_HPP_

#include "io.hpp"

namespace cpu_io {

/**
 * 时钟控制器(8253/8254)
 * @see https://en.wikipedia.org/wiki/Intel_8253
 * @see https://wiki.osdev.org/Programmable_Interval_Timer
 */
class Pit {
 public:
  /**
   * 构造函数
   * @param frequency 每秒中断次数
   */
  explicit Pit(uint16_t frequency) {
    uint16_t divisor = kMaxFrequency / frequency;

    // 设置 8253/8254 芯片工作在模式 3 下
    Out<uint8_t>(kCommand, static_cast<uint8_t>(kChannel0) |
                               static_cast<uint8_t>(kHighAndLow) |
                               static_cast<uint8_t>(kSquareWaveGenerator));

    // 分别写入低字节和高字节
    Out<uint8_t>(kChannel0Data, divisor & 0xFF);
    Out<uint8_t>(kChannel0Data, divisor >> 8);
  }

  /// @name 构造/析构函数
  /// @{
  Pit() = default;
  Pit(const Pit&) = delete;
  Pit(Pit&&) = default;
  auto operator=(const Pit&) -> Pit& = delete;
  auto operator=(Pit&&) -> Pit& = default;
  ~Pit() = default;
  /// @}

  /**
   * 计数器更新
   */
  void Ticks() { ticks_ += 1; }

  /**
   * 获取时钟中断次数
   * @return size_t 时钟中断次数
   */
  [[nodiscard]] auto GetTicks() const -> size_t { return ticks_; }

 private:
  /// 最大频率
  static constexpr size_t kMaxFrequency = 1193180;
  /// 通道 0 数据端口
  static constexpr size_t kChannel0Data = 0x40;
  /// 模式/命令端口
  static constexpr size_t kCommand = 0x43;

  /**
   * Bits         Usage
   * 6 and 7      Select channel :
   *                 0 0 = Channel 0
   *                 0 1 = Channel 1
   *                 1 0 = Channel 2
   *                 1 1 = Read-back command (8254 only)
   */
  enum Channel {
    kChannel0 = 0x0,
    kChannel1 = 0x40,
    kChannel2 = 0x80,
  };

  /**
   * Bits         Usage
   * 4 and 5      Access mode :
   *                 0 0 = Latch count value command
   *                 0 1 = Access mode: lobyte only
   *                 1 0 = Access mode: hibyte only
   *                 1 1 = Access mode: lobyte/hibyte
   */
  enum Access {
    kLatchCount = 0x0,
    kLowOnly = 0x10,
    kHighOnly = 0x20,
    kHighAndLow = 0x30,
  };

  /**
   * Bits         Usage
   * 1 to 3       Operating mode :
   *                 0 0 0 = Mode 0 (interrupt on terminal count)
   *                 0 0 1 = Mode 1 (hardware re-triggerable one-shot)
   *                 0 1 0 = Mode 2 (rate generator)
   *                 0 1 1 = Mode 3 (square wave generator)
   *                 1 0 0 = Mode 4 (software triggered strobe)
   *                 1 0 1 = Mode 5 (hardware triggered strobe)
   *                 1 1 0 = Mode 2 (rate generator, same as 010b)
   *                 1 1 1 = Mode 3 (square wave generator, same as 011b)
   * 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
   */
  enum Mode {
    kInterruptOnTerminalCount = 0x0,
    kHardwareRetriggerableOneShot = 0x2,
    kRateGenerator = 0x4,
    kSquareWaveGenerator = 0x6,
    kSoftwareTriggeredStrobe = 0x8,
    kHardwareTriggeredStrobe = 0xA,
  };

  /// 计数器
  volatile size_t ticks_ = 0;
};

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_PIT_HPP_
