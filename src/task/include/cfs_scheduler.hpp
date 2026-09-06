/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_CFS_SCHEDULER_HPP_
#define NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_CFS_SCHEDULER_HPP_

#include <cstdint>

#include "scheduler_base.hpp"
#include "nk_priority_queue"
#include "nk_vector"
#include "task_control_block.hpp"

/**
 * @brief CFS（Completely Fair Scheduler）完全公平调度器
 *
 * 📚 学习笔记：CFS 核心思想
 * ─────────────────────────────────────────────────────────────
 * Linux 从 2.6.23 起使用 CFS 作为默认调度器。核心概念是 vruntime：
 *
 *   vruntime（虚拟运行时间）= 实际运行时间 × (基准权重 / 任务权重)
 *
 * 权重越高 → vruntime 增长越慢 → 被调度越频繁 → 获得更多 CPU 时间
 *
 * 调度器总是选择 vruntime 最小的任务运行，这样每个任务获得的
 * CPU 时间与其权重成正比，实现"完全公平"。
 *
 * 理想实现用红黑树（O(log n) 插入/删除），这里用优先队列简化。
 *
 * 成员关系：
 *   CfsScheduler → SchedulerBase（纯虚基类）
 */
class CfsScheduler : public SchedulerBase {
 public:
  /// 默认权重（对应 nice 值为 0）
  static constexpr uint32_t kDefaultWeight = 1024;
  /// 抢占粒度（队列中任务的 vruntime 比当前任务小超过此值时触发抢占）
  static constexpr uint64_t kMinGranularity = 10;

  /**
   * @brief vruntime 最小堆比较器
   *
   * 使 priority_queue 的顶部始终是 vruntime 最小的任务。
   */
  struct VruntimeCompare {
    auto operator()(const TaskControlBlock* a, const TaskControlBlock* b) const
        -> bool;
  };

  /// @name 构造/析构函数
  /// @{
  CfsScheduler() = default;
  CfsScheduler(const CfsScheduler&) = delete;
  CfsScheduler(CfsScheduler&&) = delete;
  auto operator=(const CfsScheduler&) -> CfsScheduler& = delete;
  auto operator=(CfsScheduler&&) -> CfsScheduler& = delete;
  ~CfsScheduler() override = default;
  /// @}

  /**
   * @brief 将任务加入就绪队列
   *
   * 若任务 vruntime == 0（新任务），初始化为当前 min_vruntime 防止饥饿。
   * 若任务 weight == 0，设置为 kDefaultWeight。
   *
   * @param task 任务控制块指针
   */
  void Enqueue(TaskControlBlock* task) override;

  /**
   * @brief 从就绪队列中移除指定任务
   *
   * 优先队列不支持 O(1) 任意删除，需重建队列（O(n)）。
   * 生产内核用红黑树规避此问题。
   *
   * @param task 要移除的任务
   */
  void Dequeue(TaskControlBlock* task) override;

  /**
   * @brief 选择 vruntime 最小的任务运行
   *
   * 弹出堆顶任务，并更新 min_vruntime。
   *
   * @return TaskControlBlock* 下一个任务，队列为空返回 nullptr
   */
  TaskControlBlock* PickNext() override;

  /**
   * @brief 每个 tick 更新 vruntime 并检查是否需要抢占
   *
   * vruntime 增量 = kDefaultWeight × 1000 / weight
   * 若队列顶部任务的 vruntime + kMinGranularity < 当前任务的 vruntime，
   * 则触发抢占（返回 true）。
   *
   * @param current 当前运行的任务
   * @return true 需要抢占，false 继续运行
   */
  auto OnTick(TaskControlBlock* current) -> bool override;

  /**
   * @brief 任务被抢占时调用（统计计数，CFS 不需要额外处理）
   * @param task 被抢占的任务
   */
  void OnPreempted(TaskControlBlock* task) override;

  /// @brief 获取就绪队列中的任务数量
  auto GetQueueSize() const -> size_t override;

  /// @brief 判断就绪队列是否为空
  auto IsEmpty() const -> bool override;

  /// @brief 获取当前 min_vruntime（用于调试/测试）
  auto GetMinVruntime() const -> uint64_t;

 private:
  /// 就绪队列（最小堆，按 vruntime 排序）
  nk_std::priority_queue<TaskControlBlock*, nk_std::vector<TaskControlBlock*>,
                         VruntimeCompare>
      ready_queue_;

  /// 当前最小 vruntime（新任务初始化用，防止饥饿）
  uint64_t min_vruntime_ = 0;
};

#endif /* NAILONG_KERNEL_SRC_INCLUDE_SCHEDULER_CFS_SCHEDULER_HPP_ */
