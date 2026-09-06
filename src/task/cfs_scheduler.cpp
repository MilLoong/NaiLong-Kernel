/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

/**
 * @file cfs_scheduler.cpp
 * @brief CfsScheduler 实现
 *
 * 对应接口：src/task/include/cfs_scheduler.hpp
 * 存放路径：src/task/cfs_scheduler.cpp
 */

#include "cfs_scheduler.hpp"

#include "nk_cassert"

// ─────────────────────────────────────────────────────────────────
// VruntimeCompare
// ─────────────────────────────────────────────────────────────────

auto CfsScheduler::VruntimeCompare::operator()(const TaskControlBlock* a,
                                               const TaskControlBlock* b) const
    -> bool {
  // greater-than 使 priority_queue 变为最小堆（顶部是 vruntime 最小的任务）
  return a->sched_data.cfs.vruntime > b->sched_data.cfs.vruntime;
}

// ─────────────────────────────────────────────────────────────────
// Enqueue
// ─────────────────────────────────────────────────────────────────

void CfsScheduler::Enqueue(TaskControlBlock* task) {
  if (!task) {
    return;
  }

  // 新任务（vruntime == 0）初始化为 min_vruntime，防止其他任务饥饿
  if (task->sched_data.cfs.vruntime == 0) {
    task->sched_data.cfs.vruntime = min_vruntime_;
  }

  // 确保权重合法
  if (task->sched_data.cfs.weight == 0) {
    task->sched_data.cfs.weight = kDefaultWeight;
  }

  ready_queue_.push(task);
  stats_.total_enqueues++;
}

// ─────────────────────────────────────────────────────────────────
// Dequeue
// ─────────────────────────────────────────────────────────────────

void CfsScheduler::Dequeue(TaskControlBlock* task) {
  if (!task) {
    return;
  }

  // 优先队列不支持 O(1) 任意删除，需将全部元素取出后重建
  // 生产内核（Linux）用红黑树解决此问题，这里简化处理
  nk_std::vector<TaskControlBlock*> temp;
  bool found = false;

  while (!ready_queue_.empty()) {
    auto* t = ready_queue_.top();
    ready_queue_.pop();
    if (t == task) {
      found = true;
    } else {
      temp.push_back(t);
    }
  }

  for (auto* t : temp) {
    ready_queue_.push(t);
  }

  if (found) {
    stats_.total_dequeues++;
  }
}

// ─────────────────────────────────────────────────────────────────
// PickNext
// ─────────────────────────────────────────────────────────────────

TaskControlBlock* CfsScheduler::PickNext() {
  if (ready_queue_.empty()) {
    return nullptr;
  }

  auto* next = ready_queue_.top();
  ready_queue_.pop();
  stats_.total_picks++;

  // 弹出后更新 min_vruntime 为新的堆顶（或保持被选中任务的值）
  if (!ready_queue_.empty()) {
    min_vruntime_ = ready_queue_.top()->sched_data.cfs.vruntime;
  } else {
    min_vruntime_ = next->sched_data.cfs.vruntime;
  }

  return next;
}

// ─────────────────────────────────────────────────────────────────
// OnTick
// ─────────────────────────────────────────────────────────────────

auto CfsScheduler::OnTick(TaskControlBlock* current) -> bool {
  nk_assert_msg(current != nullptr,
                "CfsScheduler::OnTick: current task must not be null");

  // vruntime 增量：权重越大增长越慢，获得更多 CPU 时间
  uint64_t delta = (kDefaultWeight * 1000) / current->sched_data.cfs.weight;
  current->sched_data.cfs.vruntime += delta;

  // 检查是否需要抢占：队列顶部任务的 vruntime 比当前任务小超过阈值
  if (!ready_queue_.empty()) {
    auto* next = ready_queue_.top();
    if (next->sched_data.cfs.vruntime + kMinGranularity <
        current->sched_data.cfs.vruntime) {
      stats_.total_preemptions++;
      return true;
    }
  }

  return false;
}

// ─────────────────────────────────────────────────────────────────
// OnPreempted
// ─────────────────────────────────────────────────────────────────

void CfsScheduler::OnPreempted(TaskControlBlock* task) {
  if (task) {
    stats_.total_preemptions++;
    // CFS 不需要额外处理，Schedule() 会将任务重新 Enqueue
  }
}

// ─────────────────────────────────────────────────────────────────
// GetQueueSize / IsEmpty / GetMinVruntime
// ─────────────────────────────────────────────────────────────────

auto CfsScheduler::GetQueueSize() const -> size_t {
  return ready_queue_.size();
}

auto CfsScheduler::IsEmpty() const -> bool { return ready_queue_.empty(); }

auto CfsScheduler::GetMinVruntime() const -> uint64_t { return min_vruntime_; }
