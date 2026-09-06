/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_DRIVER_PL011_INCLUDE_PL011_H_
#define NAILONG_KERNEL_SRC_DRIVER_PL011_INCLUDE_PL011_H_

#include <console_driver.h>

#include <cstdint>

/**
 * @brief PL011 串口驱动
 *
 * PL011 是 ARM 公司设计的 UART IP 核，AArch64 平台（如树莓派、QEMU virt）标配。
 * 与 NS16550A 的主要区别：
 * - 纯 MMIO 访问（没有 x86 那种独立 I/O 端口）
 * - 寄存器更丰富（有独立的 DMA 控制、中断掩码等）
 * - 波特率由整数+分数两个寄存器共同决定（精度更高）
 *
 * @see https://developer.arm.com/documentation/ddi0183/g/
 */
class Pl011 : public ConsoleDriver {
 public:
  /**
   * 构造函数
   * @param dev_addr  设备 MMIO 基地址
   * @param clock     串口时钟频率（Hz），用于计算波特率除数
   * @param baud_rate 目标波特率（如 115200）
   */
  explicit Pl011(uint64_t dev_addr, uint64_t clock = 0, uint64_t baud_rate = 0);

  /// @name 默认构造/析构函数
  /// @{
  Pl011() = default;
  Pl011(const Pl011&) = delete;
  Pl011(Pl011&&) = default;
  auto operator=(const Pl011&) -> Pl011& = delete;
  auto operator=(Pl011&&) -> Pl011& = default;
  ~Pl011() override = default;
  /// @}

  /// 输出一个字符（等待 TX FIFO 非满后写入）
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
  /// data register
  static constexpr uint32_t kRegDR = 0x00;
  /// receive status or error clear
  static constexpr uint32_t kRegRSRECR = 0x04;
  /// DMA watermark configure
  static constexpr uint32_t kRegDmaWm = 0x08;
  /// Timeout period
  static constexpr uint32_t kRegTimeOut = 0x0C;
  /// flag register（关键！用来查发送/接收状态）
  static constexpr uint32_t kRegFR = 0x18;
  /// IrDA low-power
  static constexpr uint32_t kRegILPR = 0x20;
  /// integer baud register（波特率整数部分）
  static constexpr uint32_t kRegIBRD = 0x24;
  /// fractional baud register（波特率小数部分，精度 1/64）
  static constexpr uint32_t kRegFBRD = 0x28;
  /// line control register
  static constexpr uint32_t kRegLCRH = 0x2C;
  /// control register
  static constexpr uint32_t kRegCR = 0x30;
  /// interrupt FIFO level select
  static constexpr uint32_t kRegIFLS = 0x34;
  /// interrupt mask set/clear
  static constexpr uint32_t kRegIMSC = 0x38;
  /// raw interrupt register
  static constexpr uint32_t kRegRIS = 0x3C;
  /// masked interrupt register
  static constexpr uint32_t kRegMIS = 0x40;
  /// interrupt clear register
  static constexpr uint32_t kRegICR = 0x44;
  /// DMA control register
  static constexpr uint32_t kRegDmaCR = 0x48;

  /// flag register bits
  static constexpr uint32_t kFRRTXDIS = (1 << 13);
  static constexpr uint32_t kFRTERI = (1 << 12);
  static constexpr uint32_t kFRDDCD = (1 << 11);
  static constexpr uint32_t kFRDDSR = (1 << 10);
  static constexpr uint32_t kFRDCTS = (1 << 9);
  static constexpr uint32_t kFRRI = (1 << 8);
  static constexpr uint32_t kFRTXFE = (1 << 7);    ///< TX FIFO 空
  static constexpr uint32_t kFRRXFF = (1 << 6);    ///< RX FIFO 满
  static constexpr uint32_t kFRTxFIFO = (1 << 5);  ///< TX FIFO 满
  static constexpr uint32_t kFRRXFE =
      (1 << 4);  ///< RX FIFO 空（用于 TryGetChar）
  static constexpr uint32_t kFRBUSY = (1 << 3);  ///< UART 忙（正在发送）
  static constexpr uint32_t kFRDCD = (1 << 2);
  static constexpr uint32_t kFRDSR = (1 << 1);
  static constexpr uint32_t kFRCTS = (1 << 0);

  /// transmit/receive line register bits
  static constexpr uint32_t kLCRHSPS = (1 << 7);
  static constexpr uint32_t kLCRHWlen8 = (3 << 5);  ///< 8 数据位
  static constexpr uint32_t kLCRHWLEN_7 = (2 << 5);
  static constexpr uint32_t kLCRHWLEN_6 = (1 << 5);
  static constexpr uint32_t kLCRHWLEN_5 = (0 << 5);
  static constexpr uint32_t kLCRHFEN = (1 << 4);  ///< 使能 FIFO
  static constexpr uint32_t kLCRHSTP2 = (1 << 3);
  static constexpr uint32_t kLCRHEPS = (1 << 2);
  static constexpr uint32_t kLCRHPEN = (1 << 1);
  static constexpr uint32_t kLCRHBRK = (1 << 0);

  /// control register bits
  static constexpr uint32_t kCRCTSEN = (1 << 15);
  static constexpr uint32_t kCRRTSEN = (1 << 14);
  static constexpr uint32_t kCROUT2 = (1 << 13);
  static constexpr uint32_t kCROUT1 = (1 << 12);
  static constexpr uint32_t kCRRTS = (1 << 11);
  static constexpr uint32_t kCRDTR = (1 << 10);
  static constexpr uint32_t kCRRxEnable = (1 << 9);  ///< 使能接收
  static constexpr uint32_t kCRTxEnable = (1 << 8);  ///< 使能发送
  static constexpr uint32_t kCRLPE = (1 << 7);
  static constexpr uint32_t kCROVSFACT = (1 << 3);
  static constexpr uint32_t kCREnable = (1 << 0);  ///< 使能整个 UART

  static constexpr uint32_t kIMSCRTIM = (1 << 6);
  static constexpr uint32_t kIMSCRxim = (1 << 4);

  uint64_t base_addr_ = 0;
  uint64_t base_clock_ = 0;
  uint64_t baud_rate_ = 0;
};

#endif /* NAILONG_KERNEL_SRC_DRIVER_PL011_INCLUDE_PL011_H_ */
