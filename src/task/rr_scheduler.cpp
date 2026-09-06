/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

/**
 * @file rr_scheduler.cpp
 * @brief RoundRobinScheduler 实现
 *
 * 对应接口：src/task/include/rr_scheduler.hpp
 * 存放路径：src/task/rr_scheduler.cpp
 */

#include "rr_scheduler.hpp"

// ─────────────────────────────────────────────────────────────────
// Enqueue
// ─────────────────────────────────────────────────────────────────

void RoundRobinScheduler::Enqueue(TaskControlBlock* task) {
  if (!task) {
    return;
  }
  // 每次入队都重置时间片，确保公平
  task->sched_info.time_slice_remaining = task->sched_info.time_slice_default;
  ready_queue_.push_back(task);
  stats_.total_enqueues++;
}

// ─────────────────────────────────────────────────────────────────
// Dequeue
// ─────────────────────────────────────────────────────────────────

void RoundRobinScheduler::Dequeue(TaskControlBlock* task) {
  if (!task) {
    return;
  }
  for (auto it = ready_queue_.begin(); it != ready_queue_.end(); ++it) {
    if (*it == task) {
      ready_queue_.erase(it);
      stats_.total_dequeues++;
      break;
    }
  }
}

// ─────────────────────────────────────────────────────────────────
// PickNext
// ─────────────────────────────────────────────────────────────────

TaskControlBlock* RoundRobinScheduler::PickNext() {
  if (ready_queue_.empty()) {
    return nullptr;
  }
  auto* next = ready_queue_.front();
  ready_queue_.pop_front();
  stats_.total_picks++;
  return next;
}

// ─────────────────────────────────────────────────────────────────
// GetQueueSize / IsEmpty
// ─────────────────────────────────────────────────────────────────

auto RoundRobinScheduler::GetQueueSize() const -> size_t {
  return ready_queue_.size();
}

auto RoundRobinScheduler::IsEmpty() const -> bool {
  return ready_queue_.empty();
}

// ─────────────────────────────────────────────────────────────────
// OnTimeSliceExpired
// ─────────────────────────────────────────────────────────────────

auto RoundRobinScheduler::OnTimeSliceExpired(TaskControlBlock* task) -> bool {
  if (task) {
    // 重置时间片，Schedule() 收到 true 后会调用 Enqueue 放回队列尾部
    task->sched_info.time_slice_remaining = task->sched_info.time_slice_default;
  }
  return true;
}

// ─────────────────────────────────────────────────────────────────
// OnPreempted
// ─────────────────────────────────────────────────────────────────

void RoundRobinScheduler::OnPreempted([[maybe_unused]] TaskControlBlock* task) {
  stats_.total_preemptions++;
}
