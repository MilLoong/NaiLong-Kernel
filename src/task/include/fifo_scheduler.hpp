/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_FIFO_SCHEDULER_HPP_
#define NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_FIFO_SCHEDULER_HPP_

#include "scheduler_base.hpp"
#include "nk_list"
#include "task_control_block.hpp"

/**
 * @brief FIFO（先来先服务）调度器
 *
 * 📚 学习笔记：FIFO 调度策略
 * ─────────────────────────────────────────────────────────────
 * FIFO 是最简单的调度策略：
 *   - 任务按入队顺序排列，先入队的先运行
 *   - 无优先级、无 vruntime，纯按入队顺序
 *   - 缺点：一个长任务会阻塞后续所有任务（护航效应）
 *
 * ⚠️ 与 Linux SCHED_FIFO 的差异：本内核 TickUpdate
 * 对每种策略都做时间片 递减、耗尽即抢占（见 tick_update.cpp），因此当前 FIFO
 * 的实际行为接近 "定长时间片的 RR"。若需要真正的"不可抢占 FIFO"（运行到主动让出
 * CPU）， 需要本类覆写 OnTick/OnTimeSliceExpired 阻止时间片抢占，并让
 * TickUpdate 不对该策略递减时间片——本实现尚未提供，特此注明文档与实际行为一致。
 *
 * 成员关系：
 *   FifoScheduler → SchedulerBase（纯虚基类）
 */
class FifoScheduler : public SchedulerBase {
 public:
  /// @name 构造/析构函数
  /// @{
  FifoScheduler();
  FifoScheduler(const FifoScheduler&) = default;
  FifoScheduler(FifoScheduler&&) = default;
  auto operator=(const FifoScheduler&) -> FifoScheduler& = default;
  auto operator=(FifoScheduler&&) -> FifoScheduler& = default;
  ~FifoScheduler() override = default;
  /// @}

  /**
   * @brief 将任务加入就绪队列尾部
   * @param task 要加入的任务
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
   * @brief 任务被强制抢占时调用（统计 + 由调用方决定是否重新入队）
   *
   * 注意：本内核的时间片机制会强制抢占 FIFO 任务（见类顶部说明），
   * 该回调仅做统计，不改变 FIFO 的入队顺序语义。
   * @param task 被抢占的任务
   */
  void OnPreempted(TaskControlBlock* task) override;

 private:
  /// 就绪队列（双向链表，O(1) 头部取出、尾部插入）
  nk_std::list<TaskControlBlock*> ready_queue_;
};

#endif /* NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_FIFO_SCHEDULER_HPP_ */
