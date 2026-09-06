/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_TASK_INCLUDE_MUTEX_HPP_
#define NAILONG_KERNEL_SRC_TASK_INCLUDE_MUTEX_HPP_

#include <atomic>
#include <cstdint>
#include <limits>

#include "resource_id.hpp"
#include "task_control_block.hpp"

/**
 * @brief 互斥锁（基于任务阻塞的睡眠锁）
 *
 * 📚 学习笔记：Mutex vs SpinLock
 * ─────────────────────────────────────────────────────────────
 * SpinLock（自旋锁）：
 *   获取失败时 CPU 原地忙等（while 循环），不让出 CPU。
 *   适合：临界区极短（几条指令）、中断上下文（不能睡眠）。
 *   缺点：等待期间白白浪费 CPU 时间。
 *
 * Mutex（互斥锁）：
 *   获取失败时把当前任务加入等待队列并让出 CPU（Block），
 *   锁释放后再唤醒等待的任务（Wakeup）。
 *   适合：临界区较长、普通任务上下文。
 *   缺点：有任务切换开销，不能在中断上下文中使用。
 *
 * 本 Mutex 实现要点：
 *   - 不可重入：同一任务不能递归获取同一把锁（会返回 false）
 *   - 所有权：必须由加锁的任务解锁
 *   - 阻塞语义：Lock() 失败时调用 TaskManager::Block() 挂起当前任务
 *   - 唤醒语义：UnLock() 调用 TaskManager::Wakeup()，唤醒**该锁等待队列上的
 *     全部任务**（见 wakeup.cpp，按资源分组整体唤醒），被唤醒者重新竞争锁，
 *     失败者会再次 Block——这是"惊群式"唤醒，正确性依赖失败后的重试循环。
 *
 * ⚠️ 已知限制（与实现保持一致，见 mutex.cpp）：
 *   - 丢失唤醒防护：CAS 失败到 Block() 登记之间必须关中断，防止同核 tick
 *     抢占导致锁持有者抢先 UnLock+Wakeup（详见 mutex.cpp::Lock）。
 *   - 仅限同核：Block/Wakeup 的等待队列挂在**当前核**的 per-CPU 调度器上
 *     （block.cpp/wakeup.cpp），跨核争用同一把 Mutex 时解锁方在自己的核上
 *     找不到阻塞者，会永久丢失唤醒。SMP 场景下请勿跨核共享 Mutex。
 *
 * 使用限制：
 *   - 必须在任务上下文中使用（有当前任务概念）
 *   - 不能在中断处理程序中使用
 */
class Mutex {
 public:
  /// 锁的名称（用于调试日志）
  const char* name_{"unnamed_mutex"};

  /**
   * @brief 构造函数
   * @param name 互斥锁名称，用于调试输出
   */
  explicit Mutex(const char* name);

  /// @name 默认构造/析构函数
  /// @{
  Mutex() = default;
  Mutex(const Mutex&) = delete;
  Mutex(Mutex&&) = delete;
  auto operator=(const Mutex&) -> Mutex& = delete;
  auto operator=(Mutex&&) -> Mutex& = delete;
  ~Mutex() = default;
  /// @}

  /**
   * @brief 获取锁（阻塞式）
   *
   * 若锁已被其他任务持有，当前任务进入等待队列并让出 CPU，
   * 直到锁可用后被唤醒重试。
   *
   * 前置条件：必须在任务上下文中调用（存在当前任务）。
   * 后置条件：成功返回时，当前任务持有锁。
   *
   * @return true  成功获取锁
   * @return false 失败（递归加锁，或不在任务上下文中）
   */
  auto Lock() -> bool;

  /**
   * @brief 释放锁
   *
   * 清除 locked_ 和 owner_，然后调用 TaskManager::Wakeup 唤醒
   * 等待队列上的全部任务（惊群唤醒，见文件顶部说明）。
   *
   * 前置条件：当前任务持有锁。
   * 后置条件：锁已释放，等待该锁的任务被唤醒（若有）。
   *
   * @return true  成功释放
   * @return false 失败（当前任务不持有锁，或不在任务上下文中）
   */
  auto UnLock() -> bool;

  /**
   * @brief 尝试获取锁（非阻塞式）
   *
   * 使用 compare_exchange_strong 原子操作尝试一次，失败立即返回。
   * 不会阻塞当前任务，适合"能拿就拿，拿不到就做别的"的场景。
   *
   * @return true  成功获取锁
   * @return false 锁不可用，或递归加锁，或不在任务上下文中
   */
  auto TryLock() -> bool;

  /**
   * @brief 检查锁是否由当前任务持有
   *
   * 同时检查 locked_ == true 且 owner_ == 当前任务 pid。
   *
   * @return true  当前任务持有锁
   * @return false 未持有
   */
  [[nodiscard]] auto IsLockedByCurrentTask() const -> bool;

 protected:
  /// 锁状态（true = 已锁定）
  std::atomic<bool> locked_{false};

  /// 持有锁的任务 PID，std::numeric_limits<Pid>::max() 表示未持有
  std::atomic<Pid> owner_{std::numeric_limits<Pid>::max()};

  /// 资源 ID，用于 TaskManager::Block/Wakeup 的等待队列索引
  ResourceId resource_id_;
};

#endif /* NAILONG_KERNEL_SRC_TASK_INCLUDE_MUTEX_HPP_ */
