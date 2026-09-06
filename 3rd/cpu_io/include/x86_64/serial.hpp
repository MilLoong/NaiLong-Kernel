/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_X86_64_SERIAL_HPP_
#define CPU_IO_INCLUDE_X86_64_SERIAL_HPP_

#include "io.hpp"

namespace cpu_io {

/// @name 端口
static constexpr uint32_t kCom1 = 0x3F8;
/**
 * 串口定义
 */
class Serial {
 public:
  explicit Serial(uint32_t port) : port_(port) {
    // Disable all interrupts
    Out<uint8_t>(port_ + 1, 0x00);
    // Enable DLAB (set baud rate divisor)
    Out<uint8_t>(port_ + 3, 0x80);
    // Set divisor to 3 (lo byte) 38400 baud
    Out<uint8_t>(port_ + 0, 0x03);
    // (hi byte)
    Out<uint8_t>(port_ + 1, 0x00);
    // 8 bits, no parity, one stop bit
    Out<uint8_t>(port_ + 3, 0x03);
    // Enable FIFO, clear them, with 14-byte threshold
    Out<uint8_t>(port_ + 2, 0xC7);
    // IRQs enabled, RTS/DSR set
    Out<uint8_t>(port_ + 4, 0x0B);
    // Set in loopback mode, test the serial chip
    Out<uint8_t>(port_ + 4, 0x1E);
    // Test serial chip (send byte 0xAE and check if serial returns same byte)
    Out<uint8_t>(port_ + 0, 0xAE);
    // Check if serial is faulty (i.e: not same byte as sent)
    if (In<uint8_t>(port_ + 0) != 0xAE) {
      asm("hlt");
    }

    // If serial is not faulty set it in normal operation mode (not-loopback
    // with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    Out<uint8_t>(port_ + 4, 0x0F);
  }

  /// @name 不使用的构造函数
  /// @{
  Serial() = default;
  Serial(const Serial&) = delete;
  Serial(Serial&&) = default;
  auto operator=(const Serial&) -> Serial& = delete;
  auto operator=(Serial&&) -> Serial& = default;
  ~Serial() = default;
  /// @}

  /**
   * @brief  读一个字节
   * @return uint8_t         读取到的数据
   */
  [[nodiscard]] auto Read() const -> uint8_t {
    while (!SerialReceived()) {
      ;
    }
    return In<uint8_t>(port_);
  }

  /**
   * @brief  写一个字节
   * @param  byte              要写的数据
   */
  void Write(uint8_t byte) const {
    while (!IsTransmitEmpty()) {
      ;
    }
    Out<uint8_t>(port_, byte);
  }

 private:
  uint32_t port_;

  /**
   * @brief 串口是否接收到数据
   * @return true
   * @return false
   */
  [[nodiscard]] auto SerialReceived() const -> bool {
    return bool(In<uint8_t>(port_ + 5) & 1);
  }

  /**
   * @brief 串口是否可以发送数据
   * @return true
   * @return false
   */
  [[nodiscard]] auto IsTransmitEmpty() const -> bool {
    return bool((In<uint8_t>(port_ + 5) & 0x20) != 0);
  }
};

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_SERIAL_HPP_
