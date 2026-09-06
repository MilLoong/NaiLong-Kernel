/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include "macro.S"

.section .text

.global switch_to
.type switch_to, @function

/**
 * @brief 线程上下文切换
 *
 * void switch_to(CalleeSavedContext* prev, CalleeSavedContext* next);
 *
 * System V AMD64 ABI:
 * @param prev %rdi: 当前线程上下文
 * @param next %rsi: 下一个线程上下文
 */
switch_to:
    SaveCalleeSavedContext %rdi
    RestoreCalleeSavedContext %rsi
    // 跳转到 next->return_address（首次调度通常是 kernel_thread_entry）
    movq CTX_RETURN_ADDRESS(%rsi), %rax
    jmp *%rax

.global kernel_thread_entry
.type kernel_thread_entry, @function
kernel_thread_entry:
    // 首次进入的任务未经过 Schedule 末尾的 sti，这里打开中断
    sti
    // 恢复后约定：
    // %rbx = 真正入口函数 entry
    // %r12 = 参数 arg
    movq %r12, %rdi
    call *%rbx

    // 入口函数返回后暂时死循环，防止跑飞
1:
    pause
    jmp 1b
