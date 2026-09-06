/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 * @brief x86_64 汇编宏
 * @note 需要与 cpu_io/include/x86_64/context.hpp 中的结构体定义保持一致
 */

// clang-format off

/**
 * CalleeSavedContext 字段偏移（单位：字节）
 * uint64_t return_address;  // 0
 * uint64_t stack_pointer;   // 8
 * uint64_t entry_function;  // 16  —— rbx（首次调度时为入口函数）
 * uint64_t entry_argument;  // 24  —— r12（首次调度时为入口参数）
 * uint64_t rbp;             // 32
 * uint64_t r13;             // 40
 * uint64_t r14;             // 48
 * uint64_t r15;             // 56
 */
.equ CTX_RETURN_ADDRESS, 0
.equ CTX_STACK_POINTER, 8
.equ CTX_ENTRY_FUNCTION, 16
.equ CTX_ENTRY_ARGUMENT, 24
.equ CTX_RBP, 32
.equ CTX_R13, 40
.equ CTX_R14, 48
.equ CTX_R15, 56

/**
 * @brief 保存 callee-saved 上下文到 \base_reg 指向的结构体
 * @param base_reg 上下文基址寄存器（通常为 %rdi）
 * @note 调用约定：本宏假定仍位于 switch_to 的栈帧内，
 *       即 0(%rsp) 为返回地址。
 */
.macro SaveCalleeSavedContext base_reg
    // 保存返回地址
    movq (%rsp), %rax
    movq %rax, CTX_RETURN_ADDRESS(\base_reg)
    // 保存切换后的栈指针（跳过当前返回地址）
    leaq 8(%rsp), %rax
    movq %rax, CTX_STACK_POINTER(\base_reg)
    // 保存 rbx / r12（兼作 entry 槽位）以及其余 callee-saved
    movq %rbx, CTX_ENTRY_FUNCTION(\base_reg)
    movq %r12, CTX_ENTRY_ARGUMENT(\base_reg)
    movq %rbp, CTX_RBP(\base_reg)
    movq %r13, CTX_R13(\base_reg)
    movq %r14, CTX_R14(\base_reg)
    movq %r15, CTX_R15(\base_reg)
.endm

/**
 * @brief 从 \base_reg 指向的结构体恢复 callee-saved 上下文
 * @param base_reg 上下文基址寄存器（通常为 %rsi）
 * @note 恢复后不自动返回；由调用方 jmp 到 return_address。
 */
.macro RestoreCalleeSavedContext base_reg
    movq CTX_STACK_POINTER(\base_reg), %rsp
    movq CTX_ENTRY_FUNCTION(\base_reg), %rbx
    movq CTX_ENTRY_ARGUMENT(\base_reg), %r12
    movq CTX_RBP(\base_reg), %rbp
    movq CTX_R13(\base_reg), %r13
    movq CTX_R14(\base_reg), %r14
    movq CTX_R15(\base_reg), %r15
.endm

// clang-format on
