/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_INCLUDE_BASIC_INFO_HPP_
#define NAILONG_KERNEL_SRC_INCLUDE_BASIC_INFO_HPP_

#include <cstddef>
#include <cstdint>

#include "kernel_log.hpp"
#include "singleton.hpp"
#include "nk_iostream"

// 引用链接脚本中的变量
/// @see http://wiki.osdev.org/Using_Linker_Script_Values
/// 内核开始
extern "C" void* __executable_start[];
/// 代码段结束
extern "C" void* __etext[];
/// 内核结束
extern "C" void* end[];
/// 内核入口，在 boot.S 中定义
extern "C" void _boot();

struct BasicInfo {
  /// physical_memory 地址
  uint64_t physical_memory_addr;
  /// physical_memory 大小
  size_t physical_memory_size;

  /// kernel 地址
  uint64_t kernel_addr;
  /// kernel 大小
  size_t kernel_size;

  /// elf 地址
  uint64_t elf_addr;

  /// fdt 地址
  uint64_t fdt_addr;

  /// cpu 核数
  size_t core_count;

  /// 时钟频率
  size_t interval;

  /**
   * 构造函数，在 arch_main.cpp 中定义
   * @param argc 同 _start
   * @param argv 同 _start
   */
  explicit BasicInfo(int argc, const char** argv);

  /// @name 构造/析构函数
  /// @{
  BasicInfo() = default;
  BasicInfo(const BasicInfo&) = default;
  BasicInfo(BasicInfo&&) = default;
  auto operator=(const BasicInfo&) -> BasicInfo& = default;
  auto operator=(BasicInfo&&) -> BasicInfo& = default;
  ~BasicInfo() = default;
  /// @}

  friend auto operator<<(nk_std::ostream& ostream, const BasicInfo& basic_info)
      -> nk_std::ostream& {
    // 64 位地址用 %llX，避免被截断
    klog::Info("physical_memory_addr: 0x%llX, size 0x%llX.\n",
               static_cast<unsigned long long>(basic_info.physical_memory_addr),
               static_cast<unsigned long long>(basic_info.physical_memory_size));
    klog::Info("kernel_addr: 0x%llX, size 0x%llX.\n",
               static_cast<unsigned long long>(basic_info.kernel_addr),
               static_cast<unsigned long long>(basic_info.kernel_size));
    klog::Info("elf_addr: 0x%llX\n",
               static_cast<unsigned long long>(basic_info.elf_addr));
    klog::Info("fdt_addr: 0x%llX\n",
               static_cast<unsigned long long>(basic_info.fdt_addr));
    klog::Info("core_count: %d\n", basic_info.core_count);
    return ostream;
  }
};

#endif /* NAILONG_KERNEL_SRC_INCLUDE_BASIC_INFO_HPP_ */
