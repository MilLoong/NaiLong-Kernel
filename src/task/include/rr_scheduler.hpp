/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_RR_SCHEDULER_HPP_
#define NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_RR_SCHEDULER_HPP_

#include "scheduler_base.hpp"
#include "nk_list"
#include "task_control_block.hpp"

/**
 * @brief Round-Robin（时间片轮转）调度器
 *
 * 📚 学习笔记：RR 调度策略
 * ─────────────────────────────────────────────────────────────
 * RR 是 FIFO 的改进版，解决了护航效应：
 *   - 每个任务分配固定时间片（time_slice_default）
 *   - 时间片耗尽后，任务被放回队列尾部，轮到下一个任务
 *   - 所有任务权重相同，获得等量 CPU 时间
 *   - 适合通用分时系统（交互式任务响应好）
 *
 * 与 CFS 的区别：
 *   - RR：时间片固定，公平但不考虑优先级权重
 *   - CFS：基于 vruntime，支持加权公平，是 Linux 默认策略
 *
 * 对应 Linux SCHED_RR 实时调度策略。
 *
 * 成员关系：
 *   RoundRobinScheduler → SchedulerBase（纯虚基类）
 */
class RoundRobinScheduler : public SchedulerBase {
 public:
  /// @name 构造/析构函数
  /// @{
  RoundRobinScheduler() = default;
  RoundRobinScheduler(const RoundRobinScheduler&) = default;
  RoundRobinScheduler(RoundRobinScheduler&&) = default;
  auto operator=(const RoundRobinScheduler&) -> RoundRobinScheduler& = default;
  auto operator=(RoundRobinScheduler&&) -> RoundRobinScheduler& = default;
  ~RoundRobinScheduler() override = default;
  /// @}

  /**
   * @brief 将任务加入就绪队列尾部并重置其时间片
   *
   * 重置 time_slice_remaining = time_slice_default，确保每次入队
   * 都获得完整时间片。
   *
   * @param task 任务控制块指针
   */
  void Enqueue(TaskControlBlock* task) override;

  /**
   * @brief 从就绪队列中移除指定任务
   * @param task 要移除的任务
   */
  void Dequeue(TaskControlBlock* task) override;

  /**
   * @brief 取出队列头部的任务运行
   * @return TaskControlBlock* 下一个任务，队列为空返回 nullptr
   */
  TaskControlBlock* PickNext() override;

  /// @brief 获取就绪队列中的任务数量
  auto GetQueueSize() const -> size_t override;

  /// @brief 判断就绪队列是否为空
  auto IsEmpty() const -> bool override;

  /**
   * @brief 时间片耗尽处理：重置时间片，返回 true 要求重新入队
   *
   * Schedule() 收到 true 后会调用 Enqueue 将任务放回队列尾部。
   *
   * @param task 时间片耗尽的任务
   * @return true 始终返回 true（RR 必须重新入队）
   */
  auto OnTimeSliceExpired(TaskControlBlock* task) -> bool override;

  /**
   * @brief 任务被抢占时调用（统计计数）
   * @param task 被抢占的任务
   */
  void OnPreempted(TaskControlBlock* task) override;

 private:
  /// 就绪队列（双向链表，O(1) 头部取出、尾部插入）
  nk_std::list<TaskControlBlock*> ready_queue_;
};

#endif /* NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_RR_SCHEDULER_HPP_ */
