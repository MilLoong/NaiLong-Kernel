/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_DRIVER_NS16550A_INCLUDE_NS16550A_H_
#define NAILONG_KERNEL_SRC_DRIVER_NS16550A_INCLUDE_NS16550A_H_

#include <console_driver.h>

#include <cstdint>

/**
 * @brief NS16550A 串口驱动
 *
 * NS16550A 是最经典的 UART 芯片，x86 PC 和大多数 RISC-V 开发板都用它。
 *
 * - x86_64：通过 I/O 端口访问（CPU 的 in/out 指令，端口地址如 0x3F8）
 * - RISC-V：通过 MMIO 访问（把寄存器映射到内存地址，直接读写）
 *
 * 两种访问方式的寄存器布局完全一样，只是读写手段不同。
 */
class Ns16550a : public ConsoleDriver {
 public:
  /**
   * 构造函数
   * @param dev_addr 设备地址（x86 为 I/O 端口基址，RISC-V 为 MMIO 基址）
   */
  explicit Ns16550a(uint64_t dev_addr);

  /// @name 默认构造/析构函数
  /// @{
  Ns16550a() = default;
  Ns16550a(const Ns16550a&) = delete;
  Ns16550a(Ns16550a&&) = default;
  auto operator=(const Ns16550a&) -> Ns16550a& = delete;
  auto operator=(Ns16550a&&) -> Ns16550a& = default;
  ~Ns16550a() override = default;
  /// @}

  /// 输出一个字符（等待发送缓冲区空后写入）
  void PutChar(uint8_t c) const override;

  /**
   * 阻塞式读取一个字符
   * @return 读取到的字符
   */
  [[nodiscard]] auto GetChar() const -> uint8_t override;

  /**
   * 非阻塞式尝试读取一个字符
   * @return 读取到的字符，如果没有数据则返回 0xFF（即 uint8_t(-1)）
   */
  [[nodiscard]] auto TryGetChar() const -> uint8_t override;

 private:
  /// read mode: Receive holding reg
  static constexpr uint8_t kRegRHR = 0;
  /// write mode: Transmit Holding Reg
  static constexpr uint8_t kRegTHR = 0;
  /// write mode: interrupt enable reg
  static constexpr uint8_t kRegIER = 1;
  /// write mode: FIFO control Reg
  static constexpr uint8_t kRegFCR = 2;
  /// read mode: Interrupt Status Reg
  static constexpr uint8_t kRegISR = 2;
  /// write mode:Line Control Reg
  static constexpr uint8_t kRegLCR = 3;
  /// write mode:Modem Control Reg
  static constexpr uint8_t kRegMCR = 4;
  /// read mode: Line Status Reg
  static constexpr uint8_t kRegLSR = 5;
  /// read mode: Modem Status Reg
  static constexpr uint8_t kRegMSR = 6;

  /// LSB of divisor Latch when enabled
  static constexpr uint8_t kUartDLL = 0;
  /// MSB of divisor Latch when enabled
  static constexpr uint8_t kUartDLM = 1;

  uint64_t base_addr_;
};

#endif /* NAILONG_KERNEL_SRC_DRIVER_NS16550A_INCLUDE_NS16550A_H_ */
