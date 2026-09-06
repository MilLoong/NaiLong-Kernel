/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_X86_64_CONTEXT_HPP_
#define CPU_IO_INCLUDE_X86_64_CONTEXT_HPP_

#include <cstdint>

namespace cpu_io {

struct TrapContext {
  /// @todo

  // 统一的跨架构访问器方法（占位实现）
  __always_inline uint64_t& UserStackPointer() {
    static uint64_t dummy = 0;
    return dummy;
  }
  __always_inline uint64_t& ThreadPointer() {
    static uint64_t dummy = 0;
    return dummy;
  }
  __always_inline uint64_t& ReturnValue() {
    static uint64_t dummy = 0;
    return dummy;
  }
};

struct CalleeSavedContext {
  // System V AMD64 ABI callee-saved：rbx/rbp/r12-r15 + 返回地址与栈指针
  // entry_function / entry_argument 复用 rbx / r12 槽（首次调度用）
  uint64_t return_address;
  uint64_t stack_pointer;
  uint64_t entry_function;  // rbx
  uint64_t entry_argument;  // r12
  uint64_t rbp;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;

  // 跨架构访问器方法
  __always_inline uint64_t& ReturnAddress() { return return_address; }
  __always_inline uint64_t& EntryFunction() { return entry_function; }
  __always_inline uint64_t& EntryArgument() { return entry_argument; }
  __always_inline uint64_t& StackPointer() { return stack_pointer; }
};

static_assert(sizeof(CalleeSavedContext) == 64,
              "x86_64 CalleeSavedContext size must be 64 bytes");

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_CONTEXT_HPP_
