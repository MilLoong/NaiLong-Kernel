/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include <cpu_io.h>

#include <algorithm>
#include <memory>
#include <new>

#include "arch.h"
#include "basic_info.hpp"
#include "fifo_scheduler.hpp"
#include "interrupt_base.h"
#include "kernel_elf.hpp"
#include "kernel_log.hpp"
#include "per_cpu.hpp"
#include "singleton.hpp"
#include "nk_cassert"
#include "nk_cstring"
#include "nk_stdlib.h"
#include "nk_vector"
#include "spinlock.hpp"
#include "task_manager.hpp"
#include "virtual_memory.hpp"

void TaskManager::Schedule() {
  auto& cpu_sched = GetCurrentCpuSched();
  cpu_sched.lock.Lock();

  auto* current = GetCurrentTask();
  nk_assert_msg(current != nullptr, "Schedule: No current task to schedule");

  // 处理当前任务状态
  if (current->status == TaskStatus::kRunning) {
    // 将当前任务标记为就绪并重新入队（如果它还能运行）
    current->status = TaskStatus::kReady;
    auto* scheduler = cpu_sched.schedulers[current->policy];

    if (scheduler) {
      scheduler->OnPreempted(current);
      // 调度器决定如何处理被抢占的任务
      // 大多数情况下需要重新入队，除非是特殊策略
      if (scheduler->OnTimeSliceExpired(current)) {
        scheduler->Enqueue(current);
      }
    }
  }

  // 选择下一个任务 (按策略优先级: RealTime > Normal > Idle)
  // 跳过非 Ready 的脏项（例如睡眠后仍残留在就绪队列里的指针）
  TaskControlBlock* next = nullptr;
  for (auto* scheduler : cpu_sched.schedulers) {
    if (!scheduler || scheduler->IsEmpty()) {
      continue;
    }
    while (!scheduler->IsEmpty()) {
      auto* cand = scheduler->PickNext();
      if (!cand) {
        break;
      }
      if (cand->status == TaskStatus::kReady) {
        next = cand;
        break;
      }
      // 非 Ready：丢弃这次 Pick，继续找
    }
    if (next) {
      break;
    }
  }

  // 如果没有任务可运行
  if (!next) {
    // 如果当前任务仍然可以运行，继续运行它
    if (current->status == TaskStatus::kReady) {
      next = current;
    } else if (per_cpu::GetCurrentCore().idle_task != nullptr) {
      // 回退到 idle，避免 Exit/阻塞路径上 Schedule() 直接返回
      next = per_cpu::GetCurrentCore().idle_task;
      next->status = TaskStatus::kReady;
    } else {
      // 否则统计空闲时间并停在此核心上等待中断
      cpu_sched.idle_time++;
      cpu_sched.lock.UnLock();
      cpu_io::EnableInterrupt();
      while (true) {
        cpu_io::Pause();
      }
    }
  }

  // 不可运行的当前任务绝不能 next==current，否则随后会被标成 Running 并 no-op 返回
  if (current == next && current->status != TaskStatus::kReady) {
    next = per_cpu::GetCurrentCore().idle_task;
    nk_assert_msg(next != nullptr && next != current,
                  "Schedule: need idle while current is not runnable");
    next->status = TaskStatus::kReady;
  }

  const bool must_leave = (current->status != TaskStatus::kReady);

  // 切换到下一个任务
  nk_assert_msg(next != nullptr, "Schedule: next task must not be null");
  nk_assert_msg(next->status == TaskStatus::kReady,
                "Schedule: next task must be kReady");
  nk_assert_msg(!must_leave || current != next,
                "Schedule: must switch away from non-ready current");

  next->status = TaskStatus::kRunning;
  // 重置时间片（对于 RR 和 FIFO 有效，CFS 使用 vruntime 不依赖此字段）
  next->sched_info.time_slice_remaining = next->sched_info.time_slice_default;
  next->sched_info.context_switches++;
  cpu_sched.total_schedules++;

  // 调用调度器钩子
  auto* scheduler = cpu_sched.schedulers[next->policy];
  if (scheduler) {
    scheduler->OnScheduled(next);
  }

  // 更新 per-CPU running_task
  per_cpu::GetCurrentCore().running_task = next;

  cpu_sched.lock.UnLock();

  // 首次调度：main 跑在引导栈上，却把 running_task 标成了 idle。
  // 若 switch_to 保存到 idle->task_context，会毁掉 Idle 线程自己的栈/入口。
  // 用一次性 boot_context 承接 main，idle 的 InitTaskContext 保持原样。
  static bool boot_done = false;
  static cpu_io::CalleeSavedContext boot_context{};
  cpu_io::CalleeSavedContext* prev_ctx = &current->task_context;
  bool force_switch = false;
  if (!boot_done) {
    boot_done = true;
    prev_ctx = &boot_context;
    force_switch = true;
  }

  // 关中断直到切换完成，避免「已改 running_task、尚未 switch_to」窗口里嵌套调度
  cpu_io::DisableInterrupt();

  if (force_switch || current != next) {
    switch_to(prev_ctx, &next->task_context);
  } else {
    nk_assert_msg(!must_leave,
                  "Schedule: refused no-op while current was not runnable");
  }

  // 回到本任务后（或未切换）再开中断
  cpu_io::EnableInterrupt();
}
