/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

/**
 * @file fifo_scheduler.cpp
 * @brief FifoScheduler 实现
 *
 * 对应接口：src/task/include/fifo_scheduler.hpp
 * 存放路径：src/task/fifo_scheduler.cpp
 */

#include "fifo_scheduler.hpp"

// ─────────────────────────────────────────────────────────────────
// 构造函数
// ─────────────────────────────────────────────────────────────────

FifoScheduler::FifoScheduler() { name = "FIFO"; }

// ─────────────────────────────────────────────────────────────────
// Enqueue
// ─────────────────────────────────────────────────────────────────

void FifoScheduler::Enqueue(TaskControlBlock* task) {
  ready_queue_.push_back(task);
  stats_.total_enqueues++;
}

// ─────────────────────────────────────────────────────────────────
// Dequeue
// ─────────────────────────────────────────────────────────────────

void FifoScheduler::Dequeue(TaskControlBlock* task) {
  ready_queue_.remove(task);
  stats_.total_dequeues++;
}

// ─────────────────────────────────────────────────────────────────
// PickNext
// ─────────────────────────────────────────────────────────────────

TaskControlBlock* FifoScheduler::PickNext() {
  if (ready_queue_.empty()) {
    return nullptr;
  }
  TaskControlBlock* next = ready_queue_.front();
  ready_queue_.pop_front();
  stats_.total_picks++;
  return next;
}

// ─────────────────────────────────────────────────────────────────
// GetQueueSize / IsEmpty
// ─────────────────────────────────────────────────────────────────

auto FifoScheduler::GetQueueSize() const -> size_t {
  return ready_queue_.size();
}

auto FifoScheduler::IsEmpty() const -> bool { return ready_queue_.empty(); }

// ─────────────────────────────────────────────────────────────────
// OnPreempted
// ─────────────────────────────────────────────────────────────────

void FifoScheduler::OnPreempted([[maybe_unused]] TaskControlBlock* task) {
  stats_.total_preemptions++;
}
