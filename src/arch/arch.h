/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_ARCH_ARCH_H_
#define NAILONG_KERNEL_SRC_ARCH_ARCH_H_

#include <cpu_io.h>
#include <sys/cdefs.h>

#include <cstddef>
#include <cstdint>

// 在 switch.S 中定义
extern "C" void switch_to(cpu_io::CalleeSavedContext* prev,
                          cpu_io::CalleeSavedContext* next);

// 在 switch.S 中定义
extern "C" void kernel_thread_entry();

// 在 switch.S 中定义。
// ⚠️ x86_64：interrupt.S 中 trap_return
// 目前只是空标签，用户态任务 切换路径尚未接通（见 InitTaskContext 重载
// 2）；aarch64/riscv64 有完整实现。
extern "C" void trap_return(void*);

// 在 interrupt.S 中定义。
// ⚠️ x86_64 的 interrupt.S 未导出 trap_entry，仅 aarch64/riscv64
// 定义使用。
extern "C" void trap_entry();

/**
 * @brief 早期控制台初始化（由各架构在 early_console.cpp
 * 中通过全局构造函数自动完成）
 *
 * 📚 学习笔记：为什么叫"Early Console"？
 * ─────────────────────────────────────────────────────────────
 * 内核启动分两个阶段：
 *   1. 全局构造阶段（_start 之前）：C++ 运行时调用所有全局对象的构造函数
 *   2. ArchInit 阶段：内核主逻辑开始运行
 *
 * Early Console 在第 1 阶段就初始化好了，所以 ArchInit 一进来就能用
 * nk_printf / klog 打印调试信息。这对内核调试至关重要。
 *
 * 契约：
 * - 在 _start 之前（全局构造阶段），必须设置 nk_putchar 函数指针
 *   （nk_putchar 定义在 nk_stdio.h，是整个早期输出的唯一出口）
 * - 设置后 nk_printf / klog 即可用于早期调试输出
 * - 实现不能依赖：堆内存、中断系统、页表、任何其他已初始化的模块
 *
 * 各架构实现位置及方式：
 * - x86_64  : src/arch/x86_64/early_console.cpp
 *             直接写 NS16550A I/O 端口（COM1, 0x3F8）
 * - riscv64 : src/arch/riscv64/early_console.cpp
 *             通过 OpenSBI ecall (sbi_debug_console_write_byte)
 * - aarch64 : src/arch/aarch64/early_console.cpp
 *             直接写 PL011 MMIO 寄存器
 *
 * 实现模式（三个架构统一）：
 * @code
 *   namespace {
 *   struct EarlyConsole {
 *     EarlyConsole() {
 *       // 初始化串口硬件...
 *       nk_putchar = [](uint8_t c) { ... };  // 设置全局输出回调
 *     }
 *   } early_console;  // 全局静态实例，触发构造函数
 *   }  // namespace
 * @endcode
 */

/**
 * @brief 体系结构相关初始化（BSP，引导处理器）
 *
 * 调用时机：Early Console 初始化完成之后，内核主逻辑开始之前。
 *
 * 典型初始化顺序（以 x86_64 为例）：
 *   BasicInfo → KernelElf → GDT/段寄存器 → APIC
 *
 * @param argc 在不同体系结构有不同含义，同 _start
 * @param argv 在不同体系结构有不同含义，同 _start
 */
void ArchInit(int argc, const char** argv);

/**
 * @brief 体系结构相关初始化（AP，应用处理器，SMP 场景）
 *
 * 在 WakeUpOtherCores() 唤醒其余核后，每个 AP 核调用此函数完成自身初始化。
 * 前置条件：BSP 已完成 ArchInit，共享数据结构已就绪。
 */
void ArchInitSMP(int argc, const char** argv);

/**
 * @brief 唤醒其余 CPU 核心（SMP 启动）
 *
 * 📚 学习笔记：SMP 启动流程
 * ─────────────────────────────────────────────────────────────
 * x86_64 上通过 APIC 发送 SIPI（Startup IPI）信号唤醒 AP 核。
 * AP 核收到信号后从指定物理地址开始执行（实模式），
 * 经过一系列跳转最终进入 ArchInitSMP。
 *
 * 前置条件：ArchInit 已完成（APIC 已初始化）。
 */
void WakeUpOtherCores();

/**
 * @brief 体系结构相关中断初始化（BSP）
 *
 * 前置条件：ArchInit 已完成。
 * 后置条件：中断控制器就绪，可以注册和响应中断。
 *
 * @param argc 在不同体系结构有不同含义，同 _start
 * @param argv 在不同体系结构有不同含义，同 _start
 */
void InterruptInit(int argc, const char** argv);

/**
 * @brief 体系结构相关中断初始化（AP）
 *
 * 前置条件：InterruptInit（BSP）已完成。
 */
void InterruptInitSMP(int argc, const char** argv);

/**
 * @brief 初始化时钟中断（BSP）
 *
 * 前置条件：InterruptInit 已完成（时钟中断依赖中断控制器）。
 * 后置条件：时钟中断以 NAILONG_KERNEL_TICK Hz 频率触发，
 *           每次触发调用 TaskManager::TickUpdate()。
 *
 * 📚 学习笔记：时钟中断是调度器的心跳
 * ─────────────────────────────────────────────────────────────
 * 没有时钟中断，调度器就无法抢占正在运行的任务。
 * TimerInit 是连接"硬件时钟"和"任务调度"的桥梁。
 */
void TimerInit();

/// @brief 初始化时钟中断（AP，SMP 场景）
void TimerInitSMP();

/**
 * @brief 初始化内核线程的任务上下文（重载1）
 *
 * 📚 学习笔记：任务上下文是什么？
 * ─────────────────────────────────────────────────────────────
 * 当调度器切换任务时，需要保存当前任务的寄存器状态，
 * 并恢复下一个任务的寄存器状态。这些寄存器状态就叫"任务上下文"。
 *
 * CalleeSavedContext 只保存"被调用者保存寄存器"（Callee-Saved Registers），
 * 即函数调用约定规定由被调用函数负责保存的寄存器。
 * x86_64 上包括：rbx, rbp, r12-r15, rsp, rip 等。
 *
 * @param task_context 指向任务上下文的指针（输出参数，由此函数填充）
 * @param entry        线程入口函数
 * @param arg          传递给线程的参数
 * @param stack_top    内核栈顶地址
 */
void InitTaskContext(cpu_io::CalleeSavedContext* task_context,
                     void (*entry)(void*), void* arg, uint64_t stack_top);

/**
 * @brief 初始化用户线程的任务上下文（重载2）
 *
 * 用户线程需要额外的 TrapContext（保存用户态寄存器），
 * 内核栈上预留 TrapContext 空间，切换回用户态时通过 trap_return 恢复。
 *
 * @param task_context      指向任务上下文的指针
 * @param trap_context_ptr  指向 Trap 上下文的指针（位于内核栈上）
 * @param stack_top         内核栈顶地址
 */
void InitTaskContext(cpu_io::CalleeSavedContext* task_context,
                     cpu_io::TrapContext* trap_context_ptr, uint64_t stack_top);

/// 最多回溯 128 层调用栈
static constexpr size_t kMaxFrameCount = 128;

/**
 * @brief 获取调用栈
 *
 * 通过遍历栈帧链表（frame pointer chain）收集返回地址。
 * x86_64 上每个栈帧结构为：[saved rbp][return address]
 * 需要编译时开启帧指针（-fno-omit-frame-pointer）。
 *
 * @param buffer 指向一个数组，该数组用于存储调用栈中的返回地址
 * @param size   数组的大小，即最多存储多少个返回地址，不超过 kMaxFrameCount
 * @return       成功时返回实际写入数组中的地址数量，失败时返回 -1
 */
__always_inline auto backtrace(void** buffer, int size) -> int;

/**
 * @brief 打印调用栈（结合 KernelElf 符号表将地址解析为函数名）
 *
 * 前置条件：KernelElf 已初始化（ArchInit 中完成）。
 * 常用于内核 panic 时定位崩溃位置。
 */
void DumpStack();

#endif /* NAILONG_KERNEL_SRC_ARCH_ARCH_H_ */
