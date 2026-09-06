/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include "kernel_log.hpp"
#include "task_manager.hpp"

void TaskManager::TickUpdate() {
  auto& cpu_sched = GetCurrentCpuSched();

  bool need_preempt = false;

  {
    LockGuard<SpinLock> lock_guard(cpu_sched.lock);

    // 递增本核心的 tick 计数
    cpu_sched.local_tick++;

    auto* current = GetCurrentTask();

    // 唤醒睡眠队列中到时间的任务
    while (!cpu_sched.sleeping_tasks.empty()) {
      auto* task = cpu_sched.sleeping_tasks.top();

      // 未到唤醒时间：队首即最早者，后面的更晚
      if (task->sched_info.wake_tick > cpu_sched.local_tick) {
        break;
      }

      cpu_sched.sleeping_tasks.pop();
      task->status = TaskStatus::kReady;

      // 当前任务若在本路径被唤醒，不必入队；否则重新进入就绪队列
      if (task != current) {
        auto* scheduler = cpu_sched.schedulers[task->policy];
        if (scheduler) {
          scheduler->Enqueue(task);
        }
      }
      need_preempt = true;
    }

    // 更新当前任务的统计信息
    if (current && current->status == TaskStatus::kRunning) {
      current->sched_info.total_runtime++;

      if (current->sched_info.time_slice_remaining > 0) {
        current->sched_info.time_slice_remaining--;
      }

      auto* scheduler = cpu_sched.schedulers[current->policy];
      if (scheduler) {
        // 用 |= ，避免 OnTick 把「有睡眠任务刚醒」的 need_preempt 盖掉
        need_preempt = need_preempt || scheduler->OnTick(current);
      }

      if (!need_preempt && current->sched_info.time_slice_remaining == 0) {
        need_preempt = true;
      }
    }
  }

  if (need_preempt) {
    Schedule();
  }
}
