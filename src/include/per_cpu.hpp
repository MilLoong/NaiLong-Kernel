/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_INCLUDE_PER_CPU_HPP_
#define NAILONG_KERNEL_SRC_INCLUDE_PER_CPU_HPP_

#include <cpu_io.h>
#include <unistd.h>

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include "singleton.hpp"

struct TaskControlBlock;
struct CpuSchedData;

namespace per_cpu {

struct PerCpu {
  /// 核心 ID
  size_t core_id;

  /// 当前运行的任务
  TaskControlBlock* running_task = nullptr;
  /// 空闲任务
  TaskControlBlock* idle_task = nullptr;
  /// 调度数据 (RunQueue) 指针
  CpuSchedData* sched_data = nullptr;

  explicit PerCpu(size_t id) : core_id(id) {}

  /// @name 构造/析构函数
  /// @{
  PerCpu() = default;
  PerCpu(const PerCpu&) = default;
  PerCpu(PerCpu&&) = default;
  auto operator=(const PerCpu&) -> PerCpu& = default;
  auto operator=(PerCpu&&) -> PerCpu& = default;
  ~PerCpu() = default;
  /// @}
} __attribute__((aligned(NAILONG_KERNEL_PER_CPU_ALIGN_SIZE)));

static_assert(sizeof(PerCpu) <= NAILONG_KERNEL_PER_CPU_ALIGN_SIZE,
              "PerCpu size should not exceed cache line size");

static __always_inline auto GetCurrentCore() -> PerCpu& {
  return Singleton<std::array<PerCpu, NAILONG_KERNEL_MAX_CORE_COUNT>>::
      GetInstance()[cpu_io::GetCurrentCoreId()];
}

}  // namespace per_cpu

#endif /* NAILONG_KERNEL_SRC_INCLUDE_PER_CPU_HPP_ */
