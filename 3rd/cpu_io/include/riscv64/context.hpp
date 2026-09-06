/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_RISCV64_CONTEXT_HPP_
#define CPU_IO_INCLUDE_RISCV64_CONTEXT_HPP_

#include <cstdint>

namespace cpu_io {

/**
 * @brief RISC-V 64 寄存器上下文结构体
 * 包含所有通用寄存器 (x1-x31)、必要的特权级 CSR 以及浮点寄存器
 * 用于中断/异常处理 (保存完整现场)
 * 31 个通用寄存器(x1-x31) + 4 个 CSR + 32 个浮点寄存器(f0-f31) + 1 个 fcsr
 * 总计: 31 + 4 + 32 + 1 = 68 个 64 位寄存器，共 544 字节
 */
struct TrapContext {
  // x1: Return Address
  uint64_t ra;
  // x2: Stack Pointer (保存 Trap 发生时的 SP)
  uint64_t sp;
  // x3: Global Pointer
  uint64_t gp;
  // x4: Thread Pointer
  uint64_t tp;
  // x5: Temporary
  uint64_t t0;
  // x6: Temporary
  uint64_t t1;
  // x7: Temporary
  uint64_t t2;
  // x8: Saved Register / Frame Pointer
  uint64_t s0;
  // x9: Saved Register
  uint64_t s1;
  // x10: Function Argument / Return Value
  uint64_t a0;
  // x11: Function Argument / Return Value
  uint64_t a1;
  // x12: Function Argument
  uint64_t a2;
  // x13: Function Argument
  uint64_t a3;
  // x14: Function Argument
  uint64_t a4;
  // x15: Function Argument
  uint64_t a5;
  // x16: Function Argument
  uint64_t a6;
  // x17: Function Argument
  uint64_t a7;
  // x18: Saved Register
  uint64_t s2;
  // x19: Saved Register
  uint64_t s3;
  // x20: Saved Register
  uint64_t s4;
  // x21: Saved Register
  uint64_t s5;
  // x22: Saved Register
  uint64_t s6;
  // x23: Saved Register
  uint64_t s7;
  // x24: Saved Register
  uint64_t s8;
  // x25: Saved Register
  uint64_t s9;
  // x26: Saved Register
  uint64_t s10;
  // x27: Saved Register
  uint64_t s11;
  // x28: Temporary
  uint64_t t3;
  // x29: Temporary
  uint64_t t4;
  // x30: Temporary
  uint64_t t5;
  // x31: Temporary
  uint64_t t6;

  // Floating Point Registers
  uint64_t f0;
  uint64_t f1;
  uint64_t f2;
  uint64_t f3;
  uint64_t f4;
  uint64_t f5;
  uint64_t f6;
  uint64_t f7;
  uint64_t f8;
  uint64_t f9;
  uint64_t f10;
  uint64_t f11;
  uint64_t f12;
  uint64_t f13;
  uint64_t f14;
  uint64_t f15;
  uint64_t f16;
  uint64_t f17;
  uint64_t f18;
  uint64_t f19;
  uint64_t f20;
  uint64_t f21;
  uint64_t f22;
  uint64_t f23;
  uint64_t f24;
  uint64_t f25;
  uint64_t f26;
  uint64_t f27;
  uint64_t f28;
  uint64_t f29;
  uint64_t f30;
  uint64_t f31;
  uint64_t fcsr;

  // Supervisor Status
  uint64_t sstatus;
  // Supervisor Exception Program Counter
  uint64_t sepc;
  // Supervisor Trap Value
  uint64_t stval;
  // Supervisor Cause
  uint64_t scause;

  // 统一的跨架构访问器方法
  __always_inline uint64_t& UserStackPointer() { return sp; }
  __always_inline uint64_t& ThreadPointer() { return tp; }
  __always_inline uint64_t& ReturnValue() { return a0; }
};

/**
 * @brief 线程切换上下文 (SwitchTo)
 * 仅包含 Callee-saved 寄存器: ra, sp, s0-s11, fs0-fs11
 * 用于函数调用间的上下文切换 (Cooperative)
 * (1 + 1 + 12 + 12) * 8 = 26 * 8 = 208 bytes.
 */
struct CalleeSavedContext {
  uint64_t ra;
  uint64_t sp;
  uint64_t s0;
  uint64_t s1;
  uint64_t s2;
  uint64_t s3;
  uint64_t s4;
  uint64_t s5;
  uint64_t s6;
  uint64_t s7;
  uint64_t s8;
  uint64_t s9;
  uint64_t s10;
  uint64_t s11;
  uint64_t fs0;
  uint64_t fs1;
  uint64_t fs2;
  uint64_t fs3;
  uint64_t fs4;
  uint64_t fs5;
  uint64_t fs6;
  uint64_t fs7;
  uint64_t fs8;
  uint64_t fs9;
  uint64_t fs10;
  uint64_t fs11;

  // 跨架构访问器方法
  __always_inline uint64_t& ReturnAddress() { return ra; }
  __always_inline uint64_t& EntryFunction() { return s0; }
  __always_inline uint64_t& EntryArgument() { return s1; }
  __always_inline uint64_t& StackPointer() { return sp; }
};

// 编译时验证结构体大小
static_assert(sizeof(TrapContext) == 544, "TrapContext size must be 544 bytes");
static_assert(sizeof(CalleeSavedContext) == 208,
              "CalleeSavedContext size must be 208 bytes");

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_RISCV64_CONTEXT_HPP_
