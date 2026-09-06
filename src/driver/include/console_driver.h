/**
 * @file console_driver.h
 * @brief 控制台驱动抽象基类
 *
 * @copyright Copyright The NaiLong-Kernel Contributors
 *
 * ═══════════════════════════════════════════════════════════════
 * 📚 学习笔记：为什么需要这个抽象层？
 * ───────────────────────────────────────────────────────────────
 * NaiLong-Kernel 支持三种架构，每种架构的串口硬件不同：
 *
 *   x86_64   → NS16550A（I/O 端口操作，outb/inb）
 *   RISC-V   → NS16550A（MMIO 操作，直接读写内存地址）
 *   AArch64  → PL011   （ARM 专用 UART，MMIO）
 *
 * 它们的功能完全一样（收发字符），但寄存器操作方式不同。
 * 通过抽象基类 ConsoleDriver，上层代码只需持有一个 ConsoleDriver* 指针，
 * 无需关心底层硬件细节（依赖倒置原则）。
 *
 * ⚠️ 接入现状：Ns16550a / Pl011
 * 已继承本基类，但内核其它模块 （early_console、nk_iostream
 * 等）目前仍直接使用具体类/硬件访问， 尚未切换到
 * ConsoleDriver*。后续接入后，上层应只依赖本接口。
 *
 * ═══════════════════════════════════════════════════════════════
 * 📚 学习笔记：纯虚函数 = 接口契约
 * ───────────────────────────────────────────────────────────────
 * `= 0` 表示纯虚函数，子类必须实现，否则无法实例化。
 * 这在编译期强制要求所有驱动都实现完整接口。
 *
 * 继承关系：
 *
 *   ConsoleDriver  （本文件，纯虚基类）
 *       ├── Ns16550a  （src/driver/ns16550a/include/ns16550a.h）
 *       └── Pl011     （src/driver/pl011/include/pl011.h）
 * ═══════════════════════════════════════════════════════════════
 */

#ifndef NAILONG_KERNEL_SRC_DRIVER_INCLUDE_CONSOLE_DRIVER_H_
#define NAILONG_KERNEL_SRC_DRIVER_INCLUDE_CONSOLE_DRIVER_H_

#include <cstdint>

/**
 * @brief 控制台驱动抽象基类
 *
 * 所有串口/控制台驱动必须继承此类并实现全部纯虚方法。
 *
 * **前置条件**：硬件已完成基本初始化（波特率、数据位等已配置）。
 * **线程安全**：实现类自行决定是否加锁，基类不作保证。
 *
 * 使用示例：
 * @code
 *   ConsoleDriver* drv = &Singleton<Ns16550a>::GetInstance();
 *   drv->PutChar('H');
 *   drv->PutChar('i');
 *   uint8_t c = drv->GetChar();  // 阻塞，直到用户输入
 * @endcode
 */
class ConsoleDriver {
 public:
  /// 虚析构函数，确保通过基类指针 delete 时正确调用子类析构
  virtual ~ConsoleDriver() = default;

  /**
   * @brief 输出一个字节到控制台
   *
   * **行为**：将字节 `c` 写入串口发送缓冲区。
   * 若发送缓冲区满，**阻塞等待**直到可写。
   *
   * **前置条件**：串口已初始化。
   * **后置条件**：字节已提交到硬件发送队列。
   *
   * @param c 要输出的字节（通常为 ASCII 字符）
   */
  virtual void PutChar(uint8_t c) const = 0;

  /**
   * @brief 阻塞式读取一个字节
   *
   * **行为**：等待串口接收缓冲区有数据，然后返回该字节。
   * 若无数据，**持续轮询**直到有数据到来（忙等待）。
   *
   * **前置条件**：串口已初始化。
   * **后置条件**：返回从硬件接收缓冲区取出的一个字节。
   *
   * @return uint8_t 接收到的字节
   *
   * 📚 注意：内核早期没有中断驱动的 I/O，只能轮询。
   *    这在正常 OS 中是不可接受的（浪费 CPU），
   *    但在 Early Console 阶段是唯一选择。
   */
  [[nodiscard]] virtual auto GetChar() const -> uint8_t = 0;

  /**
   * @brief 非阻塞式尝试读取一个字节
   *
   * **行为**：检查串口接收缓冲区，若有数据则返回，若无数据**立即返回 0xFF**。
   *
   * **前置条件**：串口已初始化。
   * **后置条件**：不修改任何状态（除了消耗缓冲区中的一个字节）。
   *
   * @return uint8_t 接收到的字节；若无数据则返回 `0xFF`（即 uint8_t(-1)）
   *
   * 📚 为什么返回 0xFF 而不是 -1？
   *    因为返回类型是 uint8_t（无符号），-1 转换后正好是 0xFF。
   *    调用方检查 `TryGetChar() == 0xFF` 来判断"无数据"。
   *    这是嵌入式/内核驱动中的常见约定。
   */
  [[nodiscard]] virtual auto TryGetChar() const -> uint8_t = 0;
};

#endif  // NAILONG_KERNEL_SRC_DRIVER_INCLUDE_CONSOLE_DRIVER_H_
