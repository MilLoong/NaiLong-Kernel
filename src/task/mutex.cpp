/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

/**
 * @file mutex.cpp
 * @brief Mutex 实现
 *
 * 对应接口：src/include/mutex.hpp
 * 存放路径：src/task/mutex.cpp
 */

#include "mutex.hpp"

#include <cpu_io.h>

#include "kernel_log.hpp"
#include "singleton.hpp"
#include "task_manager.hpp"

// ─────────────────────────────────────────────────────────────────
// 构造函数
// ─────────────────────────────────────────────────────────────────

Mutex::Mutex(const char* name)
    : name_(name),
      resource_id_(ResourceType::kMutex, reinterpret_cast<uint64_t>(this)) {}

// ─────────────────────────────────────────────────────────────────
// Lock
// ─────────────────────────────────────────────────────────────────

auto Mutex::Lock() -> bool {
  auto current_task = Singleton<TaskManager>::GetInstance().GetCurrentTask();
  if (current_task == nullptr) {
    klog::Err("Mutex::Lock: Cannot lock mutex '%s' outside task context\n",
              name_);
    return false;
  }

  Pid current_pid = current_task->pid;

  // 防止递归加锁（本实现不支持重入）
  if (IsLockedByCurrentTask()) {
    klog::Warn("Mutex::Lock: Task %zu tried to recursively lock mutex '%s'\n",
               current_pid, name_);
    return false;
  }

  // 自旋 + 阻塞：compare_exchange_weak 失败则挂起当前任务，
  // 被 Wakeup 唤醒后重新尝试，直到成功。
  //
  // 丢失唤醒防护：从 CAS 失败到 Block() 把本任务登记进阻塞队列之间存在
  // 窗口。若这段窗口内发生同核调度，锁持有者可能已 UnLock+Wakeup（Wakeup
  // 只查询已登记的队列），本次唤醒就会永久丢失。因此在 Block 前后关中断，
  // 禁止本核在该窗口内被 tick 抢占（跨核场景不支持，见 mutex.hpp 顶部说明）。
  bool expected = false;
  while (!locked_.compare_exchange_weak(
      expected, true, std::memory_order_acquire, std::memory_order_relaxed)) {
    klog::Debug("Mutex::Lock: Task %zu blocking on mutex '%s'\n", current_pid,
                name_);
    cpu_io::DisableInterrupt();
    Singleton<TaskManager>::GetInstance().Block(resource_id_);
    cpu_io::EnableInterrupt();
    expected = false;  // compare_exchange_weak 失败后会修改 expected，需重置
  }

  // 加锁成功，记录所有者
  owner_.store(current_pid, std::memory_order_release);
  klog::Debug("Mutex::Lock: Task %zu acquired mutex '%s'\n", current_pid,
              name_);
  return true;
}

// ─────────────────────────────────────────────────────────────────
// UnLock
// ─────────────────────────────────────────────────────────────────

auto Mutex::UnLock() -> bool {
  auto current_task = Singleton<TaskManager>::GetInstance().GetCurrentTask();
  if (current_task == nullptr) {
    klog::Err("Mutex::UnLock: Cannot unlock mutex '%s' outside task context\n",
              name_);
    return false;
  }

  Pid current_pid = current_task->pid;

  // 只有持有锁的任务才能解锁
  if (!IsLockedByCurrentTask()) {
    klog::Warn(
        "Mutex::UnLock: Task %zu tried to unlock mutex '%s' it doesn't own\n",
        current_pid, name_);
    return false;
  }

  // 先清除 owner_，再清除 locked_（顺序很重要，防止竞态）
  owner_.store(std::numeric_limits<Pid>::max(), std::memory_order_release);
  locked_.store(false, std::memory_order_release);

  klog::Debug("Mutex::UnLock: Task %zu released mutex '%s'\n", current_pid,
              name_);

  // 唤醒等待此锁的一个任务
  Singleton<TaskManager>::GetInstance().Wakeup(resource_id_);

  return true;
}

// ─────────────────────────────────────────────────────────────────
// TryLock
// ─────────────────────────────────────────────────────────────────

auto Mutex::TryLock() -> bool {
  auto current_task = Singleton<TaskManager>::GetInstance().GetCurrentTask();
  if (current_task == nullptr) {
    klog::Err(
        "Mutex::TryLock: Cannot trylock mutex '%s' outside task context\n",
        name_);
    return false;
  }

  Pid current_pid = current_task->pid;

  if (IsLockedByCurrentTask()) {
    klog::Debug(
        "Mutex::TryLock: Task %zu tried to recursively trylock mutex '%s'\n",
        current_pid, name_);
    return false;
  }

  // 只尝试一次，不阻塞
  bool expected = false;
  if (locked_.compare_exchange_strong(expected, true, std::memory_order_acquire,
                                      std::memory_order_relaxed)) {
    owner_.store(current_pid, std::memory_order_release);
    klog::Debug("Mutex::TryLock: Task %zu acquired mutex '%s'\n", current_pid,
                name_);
    return true;
  }

  klog::Debug("Mutex::TryLock: Task %zu failed to acquire mutex '%s'\n",
              current_pid, name_);
  return false;
}

// ─────────────────────────────────────────────────────────────────
// IsLockedByCurrentTask
// ─────────────────────────────────────────────────────────────────

auto Mutex::IsLockedByCurrentTask() const -> bool {
  auto current_task = Singleton<TaskManager>::GetInstance().GetCurrentTask();
  if (current_task == nullptr) {
    return false;
  }
  return locked_.load(std::memory_order_acquire) &&
         owner_.load(std::memory_order_acquire) == current_task->pid;
}
