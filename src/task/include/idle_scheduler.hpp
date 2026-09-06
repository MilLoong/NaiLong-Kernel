/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_IDLE_SCHEDULER_HPP_
#define NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_IDLE_SCHEDULER_HPP_

#include "scheduler_base.hpp"
#include "task_control_block.hpp"

/**
 * @brief Idle 调度器（兜底调度器）
 *
 * 📚 学习笔记：Idle 任务的作用
 * ─────────────────────────────────────────────────────────────
 * 当系统中没有任何可运行任务时，CPU 不能停下来什么都不做
 * （那会导致调度器逻辑崩溃）。因此每个 CPU 核都有一个专属的
 * idle 任务，它在"无事可做"时运行，通常执行：
 *
 *   while (true) { cpu_io::WaitForInterrupt(); }  // x86: hlt 指令
 *
 * hlt 指令让 CPU 进入低功耗等待状态，直到下一个中断到来（如时钟中断），
 * 届时调度器重新检查是否有新任务就绪。
 *
 * IdleScheduler 的特点：
 *   - 只持有一个 idle 任务指针（不是队列）
 *   - PickNext 不把任务从"队列"里移除（idle 任务永远可用）
 *   - OnTick 和 OnTimeSliceExpired 始终返回 false（不触发重调度）
 *   - 最低优先级：只有 CFS/FIFO/RR 都没有任务时才被选中
 *
 * 成员关系：
 *   IdleScheduler → SchedulerBase（纯虚基类）
 */
class IdleScheduler : public SchedulerBase {
 public:
  /// @name 构造/析构函数
  /// @{
  IdleScheduler();
  ~IdleScheduler() override = default;
  /// @}

  /**
   * @brief 注册 idle 任务（通常只调用一次）
   * @param task idle 任务的控制块指针
   */
  void Enqueue(TaskControlBlock* task) override;

  /**
   * @brief 注销 idle 任务
   * @param task 要移除的任务（必须与当前 idle_task_ 相同才会生效）
   */
  void Dequeue(TaskControlBlock* task) override;

  /**
   * @brief 返回 idle 任务（不从"队列"移除，始终可用）
   * @return TaskControlBlock* idle 任务，若未注册则返回 nullptr
   */
  TaskControlBlock* PickNext() override;

  /// @brief 返回 0 或 1（取决于 idle 任务是否已注册）
  auto GetQueueSize() const -> size_t override;

  /// @brief 若 idle 任务未注册则返回 true
  auto IsEmpty() const -> bool override;

  /**
   * @brief Tick 更新（idle 任务无时间片概念，始终返回 false）
   * @return false 不触发重调度
   */
  auto OnTick(TaskControlBlock* current) -> bool override;

  /**
   * @brief 时间片耗尽（idle 任务不使用时间片，始终返回 false）
   * @return false 不需要重新入队
   */
  auto OnTimeSliceExpired(TaskControlBlock* task) -> bool override;

  /**
   * @brief 任务被抢占时调用（统计计数，idle 任务无需特殊处理）
   * @param task 被抢占的任务
   */
  void OnPreempted(TaskControlBlock* task) override;

  /**
   * @brief 任务被调度时调用（idle 任务无需特殊处理）
   * @param task 被调度的任务
   */
  void OnScheduled(TaskControlBlock* task) override;

 private:
  /// idle 任务指针（每个 CPU 核一个，不用队列）
  TaskControlBlock* idle_task_ = nullptr;
};

#endif /* NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_IDLE_SCHEDULER_HPP_ */
