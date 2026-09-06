/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_X86_64_PIC_HPP_
#define CPU_IO_INCLUDE_X86_64_PIC_HPP_

#include "io.hpp"

namespace cpu_io {

/**
 * 中断控制器(8259A)
 * @see https://wiki.osdev.org/8259_PIC
 * @note master 处理 8 个中断，slave 处理八个中断
 * @note 工作在 8086 模式下，中断处理完后需要通知 pic 重置 ISR 寄存器
 */
class Pic {
 public:
  /**
   * 构造函数
   * @param offset1 主片中断偏移，共 8 个
   * @param offset2 从片中断偏移，共 8 个
   */
  explicit Pic(uint8_t offset1, uint8_t offset2)
      : offset1_(offset1), offset2_(offset2) {
    // 0001 0001
    Out<uint8_t>(kMasterCommandPort, kIcw1Init | kIcw1Icw4);
    // 设置主片 IRQ 从 offset1_ 号中断开始
    Out<uint8_t>(kMasterDataPort, offset1_);
    // 设置主片 IR2 引脚连接从片
    // 4: 0000 0100
    Out<uint8_t>(kMasterDataPort, 4);
    // 设置主片按照 8086 的方式工作
    Out<uint8_t>(kMasterDataPort, kIcw48086);

    Out<uint8_t>(kSlaveCommandPort, kIcw1Init | kIcw1Icw4);
    // 设置从片 IRQ 从 offset2_ 号中断开始
    Out<uint8_t>(kPic2DataPort, offset2_);
    // 告诉从片输出引脚和主片 IR2 号相连
    // 2: 0000 0010
    Out<uint8_t>(kPic2DataPort, 2);
    // 设置从片按照 8086 的方式工作
    Out<uint8_t>(kPic2DataPort, kIcw48086);

    // 关闭所有中断
    Out<uint8_t>(kMasterDataPort, 0xFF);
    Out<uint8_t>(kPic2DataPort, 0xFF);
  }

  /// @name 构造/析构函数
  /// @{
  Pic() = default;
  Pic(const Pic&) = delete;
  Pic(Pic&&) = default;
  auto operator=(const Pic&) -> Pic& = delete;
  auto operator=(Pic&&) -> Pic& = default;
  ~Pic() = default;
  /// @}

  /**
   * 开启 pic 的 no 中断
   * @param no 中断号
   */
  void Enable(uint8_t no) const {
    uint8_t mask = 0;
    if (no >= offset2_) {
      mask = ((In<uint8_t>(kPic2DataPort)) & (~(1 << (no % 8))));
      Out<uint8_t>(kPic2DataPort, mask);
    } else {
      mask = ((In<uint8_t>(kMasterDataPort)) & (~(1 << (no % 8))));
      Out<uint8_t>(kMasterDataPort, mask);
    }
  }

  /**
   * 关闭 8259A 芯片的所有中断
   */
  static void Disable() {
    // 屏蔽所有中断
    Out<uint8_t>(kMasterDataPort, 0xFF);
    Out<uint8_t>(kPic2DataPort, 0xFF);
  }

  /**
   * 关闭 pic 的 no 中断
   * @param no 中断号
   */
  void Disable(uint8_t no) const {
    uint8_t mask = 0;
    if (no >= offset2_) {
      mask = ((In<uint8_t>(kPic2DataPort)) | (1 << (no % 8)));
      Out<uint8_t>(kPic2DataPort, mask);
    } else {
      mask = ((In<uint8_t>(kMasterDataPort)) | (1 << (no % 8)));
      Out<uint8_t>(kMasterDataPort, mask);
    }
  }

  /**
   * 通知 pic no 中断处理完毕
   * @param no 中断号
   */
  void Clear(uint8_t no) const {
    // 按照我们的设置，从 offset1_ 号中断起为用户自定义中断
    // 因为单片的 Intel 8259A 芯片只能处理 8 级中断
    // 故大于等于 offset2_ 的中断号是由从片处理的
    if (no >= offset2_) {
      // 发送重设信号给从片
      Out<uint8_t>(kSlaveCommandPort, kEoi);
    } else {
      // 发送重设信号给主片
      Out<uint8_t>(kMasterCommandPort, kEoi);
    }
  }

  /**
   * Returns the combined value of the cascaded PICs irq request register
   * @return uint16_t 值
   */
  static auto GetIrr() -> uint16_t { return GetIrqReg(kOcw3ReadIrr); }

  /**
   * Returns the combined value of the cascaded PICs in-service register
   * @return uint16_t 值
   */
  static auto GetIsr() -> uint16_t { return GetIrqReg(kOcw3ReadIsr); }

 private:
  uint8_t offset1_;
  uint8_t offset2_;

  /// Master (IRQs 0-7)
  static constexpr uint8_t kMaster = 0x20;
  /// Slave  (IRQs 8-15)
  static constexpr uint8_t kSlave = 0xA0;
  static constexpr uint8_t kMasterCommandPort = kMaster;
  static constexpr uint8_t kMasterDataPort = kMaster + 1;
  static constexpr uint8_t kSlaveCommandPort = kSlave;
  static constexpr uint8_t kPic2DataPort = kSlave + 1;
  /// End-of-interrupt command code
  static constexpr uint8_t kEoi = 0x20;

  /// Indicates that ICW4 will be present
  static constexpr uint8_t kIcw1Icw4 = 0x01;
  /// Single (cascade) mode
  static constexpr uint8_t kIcw1Single = 0x02;
  /// Call address interval 4 (8)
  static constexpr uint8_t kIcw1Interval4 = 0x04;
  /// Level triggered (edge) mode
  static constexpr uint8_t kIcw1Level = 0x08;
  /// Initialization - required!
  static constexpr uint8_t kIcw1Init = 0x10;

  /// OCW3 irq ready next CMD read
  static constexpr uint8_t kOcw3ReadIrr = 0x0A;
  /// OCW3 irq service next CMD read
  static constexpr uint8_t kOcw3ReadIsr = 0x0B;

  /// 8086/88 (MCS-80/85) mode
  static constexpr uint8_t kIcw48086 = 0x01;
  /// Auto (normal) EOI
  static constexpr uint8_t kIcw4Auto = 0x02;
  /// Buffered mode/slave
  static constexpr uint8_t kIcw4BufferSlave = 0x08;
  /// Buffered mode/master
  static constexpr uint8_t kIcw4BufferMaster = 0x0C;
  /// Special fully nested (not)
  static constexpr uint8_t kIcw4Sfnm = 0x10;

  /**
   * 获取中断请求寄存器的值
   * @note OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
   * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain
   * @param ocw3 OCW3
   * @return uint16_t 值
   */
  static auto GetIrqReg(uint8_t ocw3) -> uint16_t {
    Out<uint8_t>(kMasterCommandPort, ocw3);
    Out<uint8_t>(kSlaveCommandPort, ocw3);
    return (In<uint8_t>(kSlaveCommandPort) << 8) |
           In<uint8_t>(kMasterCommandPort);
  }
};

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_PIC_HPP_
