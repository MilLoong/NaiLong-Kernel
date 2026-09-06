/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 * @brief 中断初始化
 */

#include <cpu_io.h>

#include "arch.h"
#include "interrupt.h"
#include "kernel_log.hpp"
#include "nk_cstdio"
#include "nk_iostream"
#include "task_manager.hpp"

namespace {
// 定义 APIC 时钟中断向量号（使用高优先级向量）
constexpr uint8_t kApicTimerVector = 0xF0;
// 与 NAILONG_KERNEL_TICK / sys_sleep 换算一致（期望的 tick 频率，单位 Hz）
#if !defined(NAILONG_KERNEL_TICK) || (NAILONG_KERNEL_TICK + 0) < 1
#error "NAILONG_KERNEL_TICK must be defined as a positive integer (Hz)"
#endif
constexpr uint32_t kApicTimerFrequencyHz =
    static_cast<uint32_t>(NAILONG_KERNEL_TICK);

// 定义键盘中断向量号
constexpr uint8_t kKeyboardVector = 0xF1;

/**
 * @brief APIC 时钟中断处理函数
 * @param cause 中断原因
 * @param context 中断上下文
 * @return uint64_t 返回值
 */
uint64_t ApicTimerHandler(uint64_t cause, cpu_io::TrapContext* context) {
  (void)cause;
  (void)context;

  // 必须先 EOI：TickUpdate 可能 Schedule/switch_to 走掉，若 EOI 放后面
  // 则再也发不出去，APIC 定时器会永久停摆（睡眠任务全部饿死）。
  Singleton<Apic>::GetInstance().SendEoi();

  // 驱动调度 tick：睡眠唤醒、时间片与抢占
  Singleton<TaskManager>::GetInstance().TickUpdate();
  return 0;
}

/**
 * @brief 键盘中断处理函数
 * @param cause 中断原因
 * @param context 中断上下文
 * @return uint64_t 返回值
 */
uint64_t KeyboardHandler(uint64_t cause, cpu_io::TrapContext* context) {
  klog::Info("Keyboard interrupt received, vector 0x%X\n",
             static_cast<uint32_t>(cause));

  // 先 EOI，避免后续逻辑若阻塞/调度导致键盘中断卡死
  Singleton<Apic>::GetInstance().SendEoi();

  // 读取键盘扫描码
  // 8042 键盘控制器的数据端口是 0x60
  uint8_t scancode = cpu_io::In<uint8_t>(0x60);

  // 简单的扫描码处理 - 仅显示按下的键（忽略释放事件）
  if (!(scancode & 0x80)) {  // 最高位为0表示按下
    klog::Info("Key pressed: scancode 0x%02X\n", scancode);

    // 简单的扫描码到ASCII的映射（仅作为示例）
    static const char scancode_to_ascii[] = {
        0,   27,  '1',  '2',  '3',  '4', '5', '6',  '7', '8', '9', '0',
        '-', '=', '\b', '\t', 'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
        'o', 'p', '[',  ']',  '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
        'j', 'k', 'l',  ';',  '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm',  ',',  '.',  '/', 0,   '*',  0,   ' '};

    if (scancode < sizeof(scancode_to_ascii) && scancode_to_ascii[scancode]) {
      char ascii_char = scancode_to_ascii[scancode];
      klog::Info("Key: '%c'\n", ascii_char);
    }
  }

  (void)context;
  return 0;
}

bool EnableKeyboardInterrupt(uint8_t vector) {
  klog::Info("Enabling keyboard interrupt with vector 0x%x\n", vector);

  // 键盘使用 IRQ 1 (传统 PS/2 键盘)
  constexpr uint8_t kKeyboardIrq = 1;

  // 获取当前 CPU 的 APIC ID 作为目标
  uint32_t destination_apic_id = cpu_io::GetCurrentCoreId();
  klog::Info("Target APIC ID: 0x%x\n", destination_apic_id);

  // 通过 APIC 设置 IRQ 重定向到指定向量
  auto result = Singleton<Apic>::GetInstance().SetIrqRedirection(
      kKeyboardIrq, vector, destination_apic_id, false);

  if (result.has_value()) {
    klog::Info("Keyboard interrupt enabled successfully\n");
    return true;
  } else {
    klog::Err("Failed to enable keyboard interrupt\n");
    return false;
  }
}

};  // namespace

void InterruptInit(int, const char**) {
  Singleton<Interrupt>::GetInstance().SetUpIdtr();
  klog::Info("IDT ready\n");

  // 注册中断处理函数
  Singleton<Interrupt>::GetInstance().RegisterInterruptFunc(kApicTimerVector,
                                                            ApicTimerHandler);

  // 注册键盘中断处理函数
  Singleton<Interrupt>::GetInstance().RegisterInterruptFunc(kKeyboardVector,
                                                            KeyboardHandler);

  // 启用键盘中断
  EnableKeyboardInterrupt(kKeyboardVector);

  // 启用 Local APIC 定时器（暂不置 IF，留给 Schedule 入口再开，避免抢跑）
  Singleton<Apic>::GetInstance().SetupPeriodicTimer(kApicTimerFrequencyHz,
                                                    kApicTimerVector);
  klog::Info("APIC timer armed\n");

  klog::Info("Hello InterruptInit\n");
}

void InterruptInitSMP(int, const char**) {
  Singleton<Interrupt>::GetInstance().SetUpIdtr();
  // 启用 Local APIC 定时器
  Singleton<Apic>::GetInstance().SetupPeriodicTimer(kApicTimerFrequencyHz,
                                                    kApicTimerVector);
  cpu_io::Rflags::If::Set();
  klog::Info("Hello InterruptInit SMP\n");
}
