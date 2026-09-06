/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_AARCH64_CONTEXT_HPP_
#define CPU_IO_INCLUDE_AARCH64_CONTEXT_HPP_

#include <cstdint>

namespace cpu_io {

/**
 * @brief AArch64 寄存器上下文结构体
 * 包含所有通用寄存器 (x0-x30)、必要的特权级系统寄存器以及浮点/SIMD寄存器
 * 用于中断/异常处理 (保存完整现场)
 *
 * 总计: 32 (通用+填充) + 66 (SIMD+浮点状态) + 14 (填充+系统寄存器)
 *     = 112 个 uint64_t = 896 字节 (16 字节对齐)
 */
struct TrapContext {
  // x0-x7: 参数/结果寄存器
  uint64_t x0;
  uint64_t x1;
  uint64_t x2;
  uint64_t x3;
  uint64_t x4;
  uint64_t x5;
  uint64_t x6;
  uint64_t x7;
  // x8-x15: 间接结果位置寄存器 + 临时寄存器
  uint64_t x8;
  uint64_t x9;
  uint64_t x10;
  uint64_t x11;
  uint64_t x12;
  uint64_t x13;
  uint64_t x14;
  uint64_t x15;
  // x16-x17: 过程内调用临时寄存器
  uint64_t x16;
  uint64_t x17;
  // x18: 平台寄存器 (平台 ABI 保留)
  uint64_t x18;
  // x19-x28: 被调用者保存寄存器
  uint64_t x19;
  uint64_t x20;
  uint64_t x21;
  uint64_t x22;
  uint64_t x23;
  uint64_t x24;
  uint64_t x25;
  uint64_t x26;
  uint64_t x27;
  uint64_t x28;
  // 帧指针
  uint64_t x29;
  // 返回地址
  uint64_t x30;

  // 对齐填充 (保证下面的 q 寄存器从 256 字节开始，即 32 * uint64_t)
  uint64_t _padding0;

  // SIMD & 浮点寄存器 (每个 128 位，存储为 2 个 64 位)
  // 注意: 从偏移 256 开始，16 字节对齐，可使用 STP/LDP Qn 指令
  uint64_t q0[2];
  uint64_t q1[2];
  uint64_t q2[2];
  uint64_t q3[2];
  uint64_t q4[2];
  uint64_t q5[2];
  uint64_t q6[2];
  uint64_t q7[2];
  uint64_t q8[2];
  uint64_t q9[2];
  uint64_t q10[2];
  uint64_t q11[2];
  uint64_t q12[2];
  uint64_t q13[2];
  uint64_t q14[2];
  uint64_t q15[2];
  uint64_t q16[2];
  uint64_t q17[2];
  uint64_t q18[2];
  uint64_t q19[2];
  uint64_t q20[2];
  uint64_t q21[2];
  uint64_t q22[2];
  uint64_t q23[2];
  uint64_t q24[2];
  uint64_t q25[2];
  uint64_t q26[2];
  uint64_t q27[2];
  uint64_t q28[2];
  uint64_t q29[2];
  uint64_t q30[2];
  uint64_t q31[2];

  // 浮点状态寄存器
  uint64_t fpsr;
  // 浮点控制寄存器
  uint64_t fpcr;

  // 对齐填充，保证系统寄存器块从 16 字节边界开始，便于批量加载
  uint64_t _padding1[6];

  // 异常返回地址 (Exception Link Register)
  uint64_t elr_el1;
  // 保存的处理器状态寄存器 (Saved Processor State Register)
  uint64_t spsr_el1;
  // 异常综合寄存器 (Exception Syndrome Register)
  uint64_t esr_el1;

  // 用户态栈指针
  uint64_t sp_el0;
  // 用户态线程本地存储
  uint64_t tpidr_el0;
  // 用户空间页表基址
  uint64_t ttbr0_el1;

  // 内核栈指针 (独立内核栈)
  uint64_t sp_el1;
  // 内核线程指针 (当前任务 TCB)
  uint64_t tpidr_el1;

  // 统一的跨架构访问器方法
  __always_inline uint64_t& UserStackPointer() { return sp_el0; }
  __always_inline uint64_t& ThreadPointer() { return tpidr_el0; }
  __always_inline uint64_t& ReturnValue() { return x0; }
};

/**
 * @brief 线程切换上下文 (协作式切换)
 * 包含 Callee-saved 寄存器，符合 AAPCS64 标准：
 * - 通用寄存器: x19-x30 (12个)
 * - 浮点寄存器: d8-d15 (v8-v15 低 64 位，8个)
 * - 控制寄存器: sp, pc (2个)
 *
 * 总计: 12 + 8 + 2 = 22 个 uint64_t = 176 字节
 * 用于协作式任务切换，节省内存空间
 */
struct CalleeSavedContext {
  // 被调用者保存的通用寄存器
  uint64_t x19;
  uint64_t x20;
  uint64_t x21;
  uint64_t x22;
  uint64_t x23;
  uint64_t x24;
  uint64_t x25;
  uint64_t x26;
  uint64_t x27;
  uint64_t x28;
  // 帧指针
  uint64_t x29;
  // 链接寄存器
  uint64_t x30;

  // 被调用者保存的浮点寄存器 (v8-v15 的低 64 位)
  uint64_t d8;
  uint64_t d9;
  uint64_t d10;
  uint64_t d11;
  uint64_t d12;
  uint64_t d13;
  uint64_t d14;
  uint64_t d15;

  // 栈指针 (内核线程保存 sp_el1，用户线程保存 sp_el0)
  uint64_t sp;
  // 恢复时的返回地址 (首次调度时为线程入口地址)
  uint64_t pc;

  // 跨架构访问器方法
  __always_inline uint64_t& ReturnAddress() { return pc; }
  __always_inline uint64_t& EntryFunction() { return x19; }
  __always_inline uint64_t& EntryArgument() { return x20; }
  __always_inline uint64_t& StackPointer() { return sp; }
};

// 编译时验证结构体大小
static_assert(sizeof(TrapContext) == 896, "TrapContext size must be 896 bytes");
static_assert(sizeof(CalleeSavedContext) == 176,
              "CalleeSavedContext size must be 176 bytes");

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_AARCH64_CONTEXT_HPP_
