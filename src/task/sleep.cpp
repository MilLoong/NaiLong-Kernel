/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include "kernel_log.hpp"
#include "nk_cassert"
#include "task_manager.hpp"

#include <cpu_io.h>

namespace {
/// 每秒的毫秒数
constexpr uint64_t kMillisecondsPerSecond = 1000;
}  // namespace

void TaskManager::Sleep(uint64_t ms) {
  auto& cpu_sched = GetCurrentCpuSched();

  auto* current = GetCurrentTask();
  nk_assert_msg(current != nullptr, "Sleep: No current task to sleep");
  nk_assert_msg(current->status == TaskStatus::kRunning,
                "Sleep: current task status must be kRunning");

  // 如果睡眠时间为 0，仅让出 CPU（相当于 yield）
  if (ms == 0) {
    Schedule();
    return;
  }

  // 关中断贯穿「登记睡眠 → 进入 Schedule」整段，避免 tick 在窗口内把
  // 本任务唤醒/挑回，造成 sys_sleep 几乎立刻返回。
  cpu_io::DisableInterrupt();

  {
    LockGuard<SpinLock> lock_guard(cpu_sched.lock);

    // ms → tick：与时钟中断频率 NAILONG_KERNEL_TICK（Hz）对齐，向上取整
#if !defined(NAILONG_KERNEL_TICK) || (NAILONG_KERNEL_TICK + 0) < 1
#error "NAILONG_KERNEL_TICK must be defined as a positive integer (Hz)"
#endif
    uint64_t sleep_ticks =
        (ms * static_cast<uint64_t>(NAILONG_KERNEL_TICK) + kMillisecondsPerSecond -
         1) /
        kMillisecondsPerSecond;
    if (sleep_ticks == 0) {
      sleep_ticks = 1;
    }
    current->sched_info.wake_tick = cpu_sched.local_tick + sleep_ticks;

    // 若仍在就绪队列里（重复入队等），先摘掉，否则 Schedule 可能再次 Pick 到自己
    auto* scheduler = cpu_sched.schedulers[current->policy];
    if (scheduler) {
      scheduler->Dequeue(current);
    }

    current->status = TaskStatus::kSleeping;
    cpu_sched.sleeping_tasks.push(current);
  }

  // 调度到其他任务；被唤醒后从这里返回
  Schedule();
}
