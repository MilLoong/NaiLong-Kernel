/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 * @brief 中断处理
 */

#ifndef NAILONG_KERNEL_SRC_KERNEL_ARCH_X86_64_INTERRUPT_H_
#define NAILONG_KERNEL_SRC_KERNEL_ARCH_X86_64_INTERRUPT_H_

#include <cpu_io.h>

#include <array>
#include <cstdint>

#include "apic.h"
#include "interrupt_base.h"
#include "singleton.hpp"
#include "nk_stdio.h"

class Interrupt final : public InterruptBase {
 public:
  Interrupt();

  /// @name 构造/析构函数
  /// @{
  Interrupt(const Interrupt&) = delete;
  Interrupt(Interrupt&&) = delete;
  auto operator=(const Interrupt&) -> Interrupt& = delete;
  auto operator=(Interrupt&&) -> Interrupt& = delete;
  ~Interrupt() override = default;
  /// @}

  void Do(uint64_t cause, cpu_io::TrapContext* context) override;
  void RegisterInterruptFunc(uint64_t cause, InterruptFunc func) override;
  auto SendIpi(uint64_t target_cpu_mask) -> Expected<void> override;
  auto BroadcastIpi() -> Expected<void> override;

  /**
   * @brief 初始化 idtr
   */
  void SetUpIdtr();

 private:
  /// 中断处理函数数组
  alignas(4096) static std::array<InterruptFunc,
                                  cpu_io::detail::register_info::IdtrInfo::
                                      kInterruptMaxCount> interrupt_handlers;

  alignas(4096) static std::array<
      cpu_io::detail::register_info::IdtrInfo::Idt,
      cpu_io::detail::register_info::IdtrInfo::kInterruptMaxCount> idts;
};

#endif /* NAILONG_KERNEL_SRC_KERNEL_ARCH_X86_64_INTERRUPT_H_ */
