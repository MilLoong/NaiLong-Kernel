/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

/**
 * @file idle_scheduler.cpp
 * @brief IdleScheduler 实现
 *
 * 对应接口：src/task/include/idle_scheduler.hpp
 * 存放路径：src/task/idle_scheduler.cpp
 */

#include "idle_scheduler.hpp"

// ─────────────────────────────────────────────────────────────────
// 构造函数
// ─────────────────────────────────────────────────────────────────

IdleScheduler::IdleScheduler() { name = "Idle"; }

// ─────────────────────────────────────────────────────────────────
// Enqueue / Dequeue
// ─────────────────────────────────────────────────────────────────

void IdleScheduler::Enqueue(TaskControlBlock* task) {
  idle_task_ = task;
  stats_.total_enqueues++;
}

void IdleScheduler::Dequeue(TaskControlBlock* task) {
  if (idle_task_ == task) {
    idle_task_ = nullptr;
    stats_.total_dequeues++;
  }
}

// ─────────────────────────────────────────────────────────────────
// PickNext
// ─────────────────────────────────────────────────────────────────

TaskControlBlock* IdleScheduler::PickNext() {
  if (idle_task_) {
    stats_.total_picks++;
  }
  // idle 任务不从"队列"移除，它永远保持可用状态
  return idle_task_;
}

// ─────────────────────────────────────────────────────────────────
// GetQueueSize / IsEmpty
// ─────────────────────────────────────────────────────────────────

auto IdleScheduler::GetQueueSize() const -> size_t {
  return idle_task_ ? 1 : 0;
}

auto IdleScheduler::IsEmpty() const -> bool { return idle_task_ == nullptr; }

// ─────────────────────────────────────────────────────────────────
// OnTick / OnTimeSliceExpired
// ─────────────────────────────────────────────────────────────────

auto IdleScheduler::OnTick([[maybe_unused]] TaskControlBlock* current) -> bool {
  // idle 任务没有时间片概念，永不触发重调度
  return false;
}

auto IdleScheduler::OnTimeSliceExpired([[maybe_unused]] TaskControlBlock* task)
    -> bool {
  return false;
}

// ─────────────────────────────────────────────────────────────────
// OnPreempted / OnScheduled
// ─────────────────────────────────────────────────────────────────

void IdleScheduler::OnPreempted([[maybe_unused]] TaskControlBlock* task) {
  stats_.total_preemptions++;
}

void IdleScheduler::OnScheduled([[maybe_unused]] TaskControlBlock* task) {
  // idle 任务被调度时无需特殊处理
}
