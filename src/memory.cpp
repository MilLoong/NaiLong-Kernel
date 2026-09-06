/**
 * Copyright The NaiLong-Kernel Contributors
 */

#include <cpu_io.h>

#include <algorithm>
#include <bmalloc.hpp>
#include <cstddef>

#include "arch.h"
#include "basic_info.hpp"
#include "kernel_elf.hpp"
#include "kernel_log.hpp"
#include "nk_stdlib.h"
#include "virtual_memory.hpp"

namespace {

struct BmallocLogger {
  int operator()(const char* format, ...) const {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    int result = nk_vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    klog::Err("%s", buffer);
    return result;
  }
};

bmalloc::Bmalloc<BmallocLogger>* allocator = nullptr;
}  // namespace

extern "C" void* malloc(size_t size) {
  if (allocator) {
    return allocator->malloc(size);
  }
  return nullptr;
}

extern "C" void free(void* ptr) {
  if (allocator) {
    allocator->free(ptr);
  }
}

extern "C" void* calloc(size_t num, size_t size) {
  if (allocator) {
    return allocator->calloc(num, size);
  }
  return nullptr;
}

extern "C" void* realloc(void* ptr, size_t new_size) {
  if (allocator) {
    return allocator->realloc(ptr, new_size);
  }
  return nullptr;
}

extern "C" void* aligned_alloc(size_t alignment, size_t size) {
  if (allocator) {
    return allocator->aligned_alloc(alignment, size);
  }
  return nullptr;
}

extern "C" void aligned_free(void* ptr) {
  if (allocator) {
    allocator->aligned_free(ptr);
  }
}

void MemoryInit() {
  auto allocator_addr =
      reinterpret_cast<void*>(cpu_io::virtual_memory::PageAlignUp(
          Singleton<BasicInfo>::GetInstance().elf_addr +
          Singleton<KernelElf>::GetInstance().GetElfSize()));
  auto allocator_size =
      Singleton<BasicInfo>::GetInstance().physical_memory_addr +
      Singleton<BasicInfo>::GetInstance().physical_memory_size -
      reinterpret_cast<uint64_t>(allocator_addr);

  klog::Info("bmalloc address: %p, size: 0x%lX\n", allocator_addr,
             allocator_size);

  static bmalloc::Bmalloc<BmallocLogger> bmallocator(allocator_addr,
                                                     allocator_size);
  allocator = &bmallocator;

  // 构造页表并映射物理内存
  auto& vm = Singleton<VirtualMemory>::GetInstance();

#if defined(__x86_64__)
  // 开启分页后仍需访问 Local APIC / IO APIC（不在常规 RAM 范围）
  constexpr uint64_t kLocalApicBase = 0xFEE00000;
  constexpr uint64_t kIoApicBase = 0xFEC00000;
  vm.MapMMIO(kLocalApicBase, cpu_io::virtual_memory::kPageSize)
      .or_else([](auto&& err) -> Expected<void*> {
        klog::Err("Failed to map Local APIC: %s\n", err.message());
        while (true) {
          cpu_io::Pause();
        }
        return {};
      });
  vm.MapMMIO(kIoApicBase, cpu_io::virtual_memory::kPageSize)
      .or_else([](auto&& err) -> Expected<void*> {
        klog::Err("Failed to map IO APIC: %s\n", err.message());
        while (true) {
          cpu_io::Pause();
        }
        return {};
      });
#endif

  // 初始化当前核心的虚拟内存（写页表寄存器并开分页）
  vm.InitCurrentCore();

  // 重新映射早期控制台地址（如果有的话）
  if (NAILONG_KERNEL_EARLY_CONSOLE_BASE != 0) {
    vm.MapMMIO(NAILONG_KERNEL_EARLY_CONSOLE_BASE,
               cpu_io::virtual_memory::kPageSize);
  }

  klog::Info("Memory initialization completed\n");
}

void MemoryInitSMP() {
  Singleton<VirtualMemory>::GetInstance().InitCurrentCore();
  klog::Info("SMP Memory initialization completed\n");
}
