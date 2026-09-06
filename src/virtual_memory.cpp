/**
 * Copyright The NaiLong-Kernel Contributors
 */

/**
 * @file virtual_memory.cpp
 * @brief VirtualMemory 实现
 *
 * 对应接口：src/include/virtual_memory.hpp
 * 存放路径：src/virtual_memory.cpp
 *
 * 📚 学习笔记：为什么实现放在这里而不是头文件？
 * ─────────────────────────────────────────────────────────────
 * 原来 virtual_memory.hpp 是 424 行的"header-only"实现。
 * 问题：
 *   1. 每个包含它的 .cpp 都会重新编译一遍全部实现，编译慢
 *   2. AI 看到头文件就看到了全部答案，失去了学习价值
 *   3. 修改实现必须重新编译所有依赖它的翻译单元
 *
 * 现在头文件只有声明，实现在这里，符合 C++ 分离编译的最佳实践。
 */

#include "virtual_memory.hpp"

#include <bmalloc.hpp>
#include <cstring>

#include "basic_info.hpp"
#include "kernel_log.hpp"
#include "singleton.hpp"
#include "nk_cassert"

extern "C" {
void* aligned_alloc(size_t alignment, size_t size);
void aligned_free(void* ptr);
}

// ─────────────────────────────────────────────────────────────────
// 构造函数
// ─────────────────────────────────────────────────────────────────

VirtualMemory::VirtualMemory() {
  // 分配根页表目录（必须页对齐，因为 CR3/satp 只存储页对齐地址）
  kernel_page_dir_ = aligned_alloc(cpu_io::virtual_memory::kPageSize,
                                   cpu_io::virtual_memory::kPageSize);
  nk_assert_msg(kernel_page_dir_ != nullptr,
                "Failed to allocate kernel page directory");

  // 清零页表目录（所有 PTE 初始为无效）
  std::memset(kernel_page_dir_, 0, cpu_io::virtual_memory::kPageSize);

  // 获取内核基本信息（物理内存范围）
  const auto& basic_info = Singleton<BasicInfo>::GetInstance();

  // 将全部物理内存以恒等映射（PA == VA）方式映射进内核页表
  // 这样内核代码通过虚拟地址访问内存时，MMU 翻译结果等于原物理地址
  MapMMIO(basic_info.physical_memory_addr, basic_info.physical_memory_size)
      .or_else([](auto&& err) -> Expected<void*> {
        klog::Err("Failed to map kernel memory: %s", err.message());
        while (true) {
          cpu_io::Pause();
        }
        return {};
      });

  klog::Info("Kernel memory mapped from 0x%lX to 0x%lX\n",
             basic_info.physical_memory_addr,
             basic_info.physical_memory_addr + basic_info.physical_memory_size);
}

// ─────────────────────────────────────────────────────────────────
// InitCurrentCore
// ─────────────────────────────────────────────────────────────────

void VirtualMemory::InitCurrentCore() const {
  // 把根页表目录的物理地址写入 CPU 的页表寄存器
  // x86_64: 写 CR3；RISC-V: 写 satp（同时设置 MODE 字段）
  cpu_io::virtual_memory::SetPageDirectory(
      reinterpret_cast<uint64_t>(kernel_page_dir_));

  // 开启 MMU 分页
  // x86_64: 设置 CR0.PG 位；RISC-V: satp.MODE 已在上一步设置
  cpu_io::virtual_memory::EnablePage();
}

// ─────────────────────────────────────────────────────────────────
// MapMMIO
// ─────────────────────────────────────────────────────────────────

auto VirtualMemory::MapMMIO(uint64_t phys_addr, size_t size, uint32_t flags)
    -> Expected<void*> {
  // 向下对齐起始地址，向上对齐结束地址，确保覆盖完整的页
  auto start_page = cpu_io::virtual_memory::PageAlign(phys_addr);
  auto end_page = cpu_io::virtual_memory::PageAlignUp(phys_addr + size);

  for (uint64_t addr = start_page; addr < end_page;
       addr += cpu_io::virtual_memory::kPageSize) {
    auto result = MapPage(kernel_page_dir_, reinterpret_cast<void*>(addr),
                          reinterpret_cast<void*>(addr), flags);
    if (!result.has_value()) {
      return std::unexpected(result.error());
    }
  }

  return reinterpret_cast<void*>(phys_addr);
}

// ─────────────────────────────────────────────────────────────────
// MapPage
// ─────────────────────────────────────────────────────────────────

auto VirtualMemory::MapPage(void* page_dir, void* virtual_addr,
                            void* physical_addr, uint32_t flags)
    -> Expected<void> {
  nk_assert_msg(page_dir != nullptr, "MapPage: page_dir is null");

  // 找到（或分配）叶层页表项
  auto pte_result = FindPageTableEntry(page_dir, virtual_addr, true);
  if (!pte_result.has_value()) {
    return std::unexpected(Error(ErrorCode::kVmMapFailed));
  }

  auto pte = pte_result.value();

  // 检查是否已映射
  if (cpu_io::virtual_memory::IsPageTableEntryValid(*pte)) {
    auto existing_pa = cpu_io::virtual_memory::PageTableEntryToPhysical(*pte);
    auto attr_mask = (1ULL << cpu_io::virtual_memory::kPteAttributeBits) - 1;

    if (existing_pa == reinterpret_cast<uint64_t>(physical_addr) &&
        (*pte & attr_mask) == flags) {
      // PA 和 flags 完全相同：重复映射，静默跳过
      klog::Debug("MapPage: duplicate va=%p pa=0x%lX flags=0x%X, skip\n",
                  virtual_addr, existing_pa, flags);
      return {};
    }
    // PA 或 flags 不同：打印警告后覆盖
    klog::Warn("MapPage: remap va=%p from pa=0x%lX to pa=%p\n", virtual_addr,
               existing_pa, physical_addr);
  }

  // 写入页表项，并刷新 TLB 使新映射立即生效
  *pte = cpu_io::virtual_memory::PhysicalToPageTableEntry(
      reinterpret_cast<uint64_t>(physical_addr), flags);
  cpu_io::virtual_memory::FlushTLBAll();

  return {};
}

// ─────────────────────────────────────────────────────────────────
// UnmapPage
// ─────────────────────────────────────────────────────────────────

auto VirtualMemory::UnmapPage(void* page_dir, void* virtual_addr)
    -> Expected<void> {
  nk_assert_msg(page_dir != nullptr, "UnmapPage: page_dir is null");

  auto pte_result = FindPageTableEntry(page_dir, virtual_addr, false);
  if (!pte_result.has_value()) {
    return std::unexpected(Error(ErrorCode::kVmPageNotMapped));
  }

  auto pte = pte_result.value();

  if (!cpu_io::virtual_memory::IsPageTableEntryValid(*pte)) {
    return std::unexpected(Error(ErrorCode::kVmPageNotMapped));
  }

  // 清零页表项 = 标记为无效
  *pte = 0;
  cpu_io::virtual_memory::FlushTLBAll();

  return {};
}

// ─────────────────────────────────────────────────────────────────
// GetMapping
// ─────────────────────────────────────────────────────────────────

auto VirtualMemory::GetMapping(void* page_dir, void* virtual_addr)
    -> Expected<void*> {
  nk_assert_msg(page_dir != nullptr, "GetMapping: page_dir is null");

  auto pte_result = FindPageTableEntry(page_dir, virtual_addr, false);
  if (!pte_result.has_value()) {
    return std::unexpected(Error(ErrorCode::kVmPageNotMapped));
  }

  auto pte = pte_result.value();

  if (!cpu_io::virtual_memory::IsPageTableEntryValid(*pte)) {
    return std::unexpected(Error(ErrorCode::kVmPageNotMapped));
  }

  return reinterpret_cast<void*>(
      cpu_io::virtual_memory::PageTableEntryToPhysical(*pte));
}

// ─────────────────────────────────────────────────────────────────
// DestroyPageDirectory
// ─────────────────────────────────────────────────────────────────

void VirtualMemory::DestroyPageDirectory(void* page_dir, bool free_pages) {
  if (page_dir == nullptr) {
    return;
  }

  RecursiveFreePageTable(reinterpret_cast<uint64_t*>(page_dir),
                         cpu_io::virtual_memory::kPageTableLevels - 1,
                         free_pages);

  // 释放根页表目录本身
  aligned_free(page_dir);

  klog::Debug("Destroyed page directory at %p\n", page_dir);
}

// ─────────────────────────────────────────────────────────────────
// ClonePageDirectory
// ─────────────────────────────────────────────────────────────────

auto VirtualMemory::ClonePageDirectory(void* src_page_dir, bool copy_mappings)
    -> Expected<void*> {
  nk_assert_msg(src_page_dir != nullptr,
                "ClonePageDirectory: source page directory is nullptr");

  // 分配新的根页表目录
  auto dst_page_dir = aligned_alloc(cpu_io::virtual_memory::kPageSize,
                                    cpu_io::virtual_memory::kPageSize);
  if (dst_page_dir == nullptr) {
    return std::unexpected(Error(ErrorCode::kVmAllocationFailed));
  }
  std::memset(dst_page_dir, 0, cpu_io::virtual_memory::kPageSize);

  auto result = RecursiveClonePageTable(
      reinterpret_cast<uint64_t*>(src_page_dir),
      reinterpret_cast<uint64_t*>(dst_page_dir),
      cpu_io::virtual_memory::kPageTableLevels - 1, copy_mappings);

  if (!result.has_value()) {
    // 复制失败，回收已分配的中间页表
    DestroyPageDirectory(dst_page_dir, false);
    return std::unexpected(result.error());
  }

  klog::Debug("Cloned page directory from %p to %p\n", src_page_dir,
              dst_page_dir);
  return dst_page_dir;
}

// ─────────────────────────────────────────────────────────────────
// private: RecursiveFreePageTable
// ─────────────────────────────────────────────────────────────────

void VirtualMemory::RecursiveFreePageTable(uint64_t* table, size_t level,
                                           bool free_pages) {
  if (table == nullptr) {
    return;
  }

  for (size_t i = 0; i < kEntriesPerTable; ++i) {
    uint64_t pte = table[i];
    if (!cpu_io::virtual_memory::IsPageTableEntryValid(pte)) {
      continue;
    }

    auto pa = cpu_io::virtual_memory::PageTableEntryToPhysical(pte);

    if (level > 0) {
      // 非叶层：递归释放子页表
      RecursiveFreePageTable(reinterpret_cast<uint64_t*>(pa), level - 1,
                             free_pages);
    } else if (free_pages) {
      // 叶层且要求释放物理页
      aligned_free(reinterpret_cast<void*>(pa));
    }

    table[i] = 0;
  }

  // 非根层的中间页表节点需要释放（根由 DestroyPageDirectory 负责）
  if (level < cpu_io::virtual_memory::kPageTableLevels - 1) {
    aligned_free(table);
  }
}

// ─────────────────────────────────────────────────────────────────
// private: RecursiveClonePageTable
// ─────────────────────────────────────────────────────────────────

auto VirtualMemory::RecursiveClonePageTable(uint64_t* src_table,
                                            uint64_t* dst_table, size_t level,
                                            bool copy_mappings)
    -> Expected<void> {
  nk_assert_msg(src_table != nullptr,
                "RecursiveClonePageTable: src_table is null");
  nk_assert_msg(dst_table != nullptr,
                "RecursiveClonePageTable: dst_table is null");

  for (size_t i = 0; i < kEntriesPerTable; ++i) {
    uint64_t src_pte = src_table[i];
    if (!cpu_io::virtual_memory::IsPageTableEntryValid(src_pte)) {
      continue;
    }

    if (level > 0) {
      // 非叶层：分配新的子页表并递归复制
      auto src_pa = cpu_io::virtual_memory::PageTableEntryToPhysical(src_pte);
      auto* src_next = reinterpret_cast<uint64_t*>(src_pa);

      auto* dst_next = static_cast<uint64_t*>(
          aligned_alloc(cpu_io::virtual_memory::kPageSize,
                        cpu_io::virtual_memory::kPageSize));
      if (dst_next == nullptr) {
        return std::unexpected(Error(ErrorCode::kVmAllocationFailed));
      }
      std::memset(dst_next, 0, cpu_io::virtual_memory::kPageSize);

      auto result =
          RecursiveClonePageTable(src_next, dst_next, level - 1, copy_mappings);
      if (!result.has_value()) {
        aligned_free(dst_next);
        return std::unexpected(result.error());
      }

      // 让目标页表项指向新分配的子页表
      dst_table[i] = cpu_io::virtual_memory::PhysicalToPageTableEntry(
          reinterpret_cast<uint64_t>(dst_next),
          cpu_io::virtual_memory::GetTableEntryPermissions());
    } else {
      // 叶层：按需复制页表项（共享物理页，写时复制的基础）
      if (copy_mappings) {
        dst_table[i] = src_pte;
      }
    }
  }

  return {};
}

// ─────────────────────────────────────────────────────────────────
// private: FindPageTableEntry
// ─────────────────────────────────────────────────────────────────

auto VirtualMemory::FindPageTableEntry(void* page_dir, void* virtual_addr,
                                       bool allocate) -> Expected<uint64_t*> {
  auto* current_table = reinterpret_cast<uint64_t*>(page_dir);
  auto vaddr = reinterpret_cast<uint64_t>(virtual_addr);

  // 从最高层向下遍历，每层取出该层的虚拟页号（VPN）
  for (size_t level = cpu_io::virtual_memory::kPageTableLevels - 1; level > 0;
       --level) {
    auto vpn = cpu_io::virtual_memory::GetVirtualPageNumber(vaddr, level);
    auto* pte = &current_table[vpn];

    if (cpu_io::virtual_memory::IsPageTableEntryValid(*pte)) {
      // 页表项有效，取出下一级页表的物理地址
      current_table = reinterpret_cast<uint64_t*>(
          cpu_io::virtual_memory::PageTableEntryToPhysical(*pte));
    } else if (allocate) {
      // 页表项无效且允许分配：创建新的中间层页表
      auto* new_table = static_cast<uint64_t*>(
          aligned_alloc(cpu_io::virtual_memory::kPageSize,
                        cpu_io::virtual_memory::kPageSize));
      if (new_table == nullptr) {
        return std::unexpected(Error(ErrorCode::kVmAllocationFailed));
      }
      std::memset(new_table, 0, cpu_io::virtual_memory::kPageSize);

      // 写入中间层页表项，权限用 GetTableEntryPermissions（非叶节点权限）
      *pte = cpu_io::virtual_memory::PhysicalToPageTableEntry(
          reinterpret_cast<uint64_t>(new_table),
          cpu_io::virtual_memory::GetTableEntryPermissions());

      current_table = new_table;
    } else {
      return std::unexpected(Error(ErrorCode::kVmPageNotMapped));
    }
  }

  // 到达叶层，返回叶层页表项的指针（供调用方读写）
  auto vpn = cpu_io::virtual_memory::GetVirtualPageNumber(vaddr, 0);
  return &current_table[vpn];
}
