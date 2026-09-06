/**
 * Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_MEMORY_INCLUDE_VIRTUAL_MEMORY_HPP_
#define NAILONG_KERNEL_SRC_MEMORY_INCLUDE_VIRTUAL_MEMORY_HPP_

#include <cpu_io.h>

#include <cstddef>
#include <cstdint>

#include "expected.hpp"

/**
 * @brief 虚拟内存管理类
 *
 * 负责内核页表的创建、映射、查询和销毁。
 * 使用 cpu_io 库提供的架构无关接口操作页表项，
 * 因此同一份代码可在 x86_64 / RISC-V / AArch64 上编译运行。
 *
 * 📚 学习笔记：虚拟内存的核心概念
 * ─────────────────────────────────────────────────────────────
 * 现代 CPU 通过 MMU（内存管理单元）将"虚拟地址"翻译为"物理地址"。
 * 翻译规则存储在"页表"里，页表是一棵多级树：
 *
 *   虚拟地址 → [L3 index][L2 index][L1 index][L0 index][页内偏移]
 *                  ↓          ↓          ↓          ↓
 *              页目录项   页目录项   页目录项   页表项(PTE)→ 物理页帧号
 *
 * x86_64 用 4 级页表（PML4→PDPT→PD→PT），页大小 4KB。
 * RISC-V Sv39 用 3 级页表，AArch64 类似。
 *
 * VirtualMemory 类封装了这棵树的增删查操作：
 *   - MapPage       → 在树中插入一条"虚拟→物理"映射
 *   - UnmapPage     → 删除一条映射
 *   - GetMapping    → 查询一条映射
 *   - MapMMIO       → 批量映射（用于设备内存）
 *   - ClonePageDirectory → 复制整棵树（fork 时用）
 *   - DestroyPageDirectory → 销毁整棵树（进程退出时用）
 *
 * 前置条件：BasicInfo 已初始化（构造函数需要物理内存地址和大小）。
 * 线程安全：不保证，调用方需要加锁。
 */
class VirtualMemory {
 public:
  /**
   * @brief 构造函数
   *
   * 分配根页表目录（一个对齐到页大小的页），清零后，
   * 将 BasicInfo 中描述的全部物理内存以恒等映射（PA == VA）方式映射进来。
   *
   * 前置条件：BasicInfo 单例已构造，堆分配器（bmalloc）已可用。
   * 后置条件：kernel_page_dir_ 指向一个有效的根页表，物理内存已映射。
   */
  VirtualMemory();

  /// @name 构造/析构函数
  /// @{
  VirtualMemory(const VirtualMemory&) = delete;
  VirtualMemory(VirtualMemory&&) = default;
  auto operator=(const VirtualMemory&) -> VirtualMemory& = delete;
  auto operator=(VirtualMemory&&) -> VirtualMemory& = default;
  ~VirtualMemory() = default;
  /// @}

  /**
   * @brief 将当前 CPU 核心的页表寄存器指向 kernel_page_dir_ 并开启分页
   *
   * x86_64 上写 CR3 寄存器；RISC-V 上写 satp 寄存器。
   * 前置条件：VirtualMemory 已构造，物理内存已映射（否则开启分页后立即
   * fault）。
   */
  void InitCurrentCore() const;

  /**
   * @brief 批量映射设备内存（MMIO）或普通物理内存
   *
   * 将 [phys_addr, phys_addr + size) 以恒等映射（VA == PA）方式
   * 插入 kernel_page_dir_ 页表，页属性由 flags 控制。
   *
   * @param phys_addr 物理基地址（会向下对齐到页边界）
   * @param size      映射大小（会向上对齐到页边界）
   * @param flags     页表属性，默认为内核可读写、禁止用户访问
   * @return Expected<void*> 成功返回 phys_addr（转为 void*），失败返回错误码
   *
   * 📚 为什么 MMIO 也用这个函数？
   *    设备寄存器被映射到某段物理地址，内核访问这段地址时
   *    MMU 必须知道这个映射存在，否则会触发 Page Fault。
   *    所以设备初始化前都要先调用 MapMMIO 把设备地址段映射进来。
   */
  auto MapMMIO(
      uint64_t phys_addr, size_t size,
      uint32_t flags = cpu_io::virtual_memory::GetKernelPagePermissions())
      -> Expected<void*>;

  /**
   * @brief 映射单个页面
   *
   * 在 page_dir 页表中建立 virtual_addr → physical_addr 的映射。
   * 若中间层页表不存在，自动分配并插入。
   * 若已映射且 PA 和 flags 相同，跳过（重复映射不报错）。
   * 若已映射但 PA 或 flags 不同，打印警告后覆盖。
   *
   * @param page_dir      根页表目录指针
   * @param virtual_addr  要映射的虚拟地址（需页对齐）
   * @param physical_addr 目标物理地址（需页对齐）
   * @param flags         页表项属性位
   * @return Expected<void> 成功返回空，失败返回 kVmMapFailed
   */
  auto MapPage(void* page_dir, void* virtual_addr, void* physical_addr,
               uint32_t flags) -> Expected<void>;

  /**
   * @brief 取消映射单个页面
   *
   * 将 page_dir 页表中 virtual_addr 对应的页表项清零，并刷新 TLB。
   * 不释放物理页（物理页的生命周期由物理内存分配器管理）。
   *
   * @param page_dir     根页表目录指针
   * @param virtual_addr 要取消映射的虚拟地址
   * @return Expected<void> 成功返回空，若地址未映射返回 kVmPageNotMapped
   */
  auto UnmapPage(void* page_dir, void* virtual_addr) -> Expected<void>;

  /**
   * @brief 查询虚拟地址对应的物理地址
   *
   * @param page_dir     根页表目录指针
   * @param virtual_addr 要查询的虚拟地址
   * @return Expected<void*> 成功返回物理地址（转为 void*），
   *                         若未映射返回 kVmPageNotMapped
   */
  [[nodiscard]] auto GetMapping(void* page_dir, void* virtual_addr)
      -> Expected<void*>;

  /**
   * @brief 销毁页表，递归释放所有中间层页表
   *
   * 遍历整棵页表树，释放所有中间页表节点占用的内存。
   * 若 free_pages 为 true，同时释放最后一级映射的物理页帧。
   *
   * @param page_dir   要销毁的根页表目录（调用后指针失效）
   * @param free_pages 是否同时释放映射的物理页，默认 false
   *
   * 📚 典型使用场景：
   *    进程退出时调用 DestroyPageDirectory(task->page_dir, true)
   *    释放该进程的全部地址空间。
   */
  void DestroyPageDirectory(void* page_dir, bool free_pages = false);

  /**
   * @brief 复制页表（用于 fork）
   *
   * 递归复制 src_page_dir 的所有中间层页表，
   * 生成一棵结构相同的新页表树。
   *
   * @param src_page_dir  源根页表目录
   * @param copy_mappings true：复制最后一级页表项（共享物理页，写时复制的基础）
   *                      false：只复制树结构，叶节点保持为空
   * @return Expected<void*> 成功返回新根页表目录指针，
   *                         失败返回 kVmAllocationFailed
   */
  auto ClonePageDirectory(void* src_page_dir, bool copy_mappings = true)
      -> Expected<void*>;

 private:
  /// 内核根页表目录（一个对齐到 kPageSize 的页）
  void* kernel_page_dir_ = nullptr;

  /// 每张页表包含的条目数（kPageSize / sizeof(uint64_t)）
  static constexpr size_t kEntriesPerTable =
      cpu_io::virtual_memory::kPageSize / sizeof(void*);

  /**
   * @brief 递归释放页表树
   *
   * @param table      当前层页表基地址
   * @param level      当前层级（最高层 = kPageTableLevels-1，叶层 = 0）
   * @param free_pages 是否释放叶层映射的物理页
   */
  void RecursiveFreePageTable(uint64_t* table, size_t level, bool free_pages);

  /**
   * @brief 递归复制页表树
   *
   * @param src_table    源页表
   * @param dst_table    目标页表（已分配并清零）
   * @param level        当前层级
   * @param copy_mappings 是否复制叶节点映射
   * @return Expected<void> 成功返回空，失败返回 kVmAllocationFailed
   */
  auto RecursiveClonePageTable(uint64_t* src_table, uint64_t* dst_table,
                               size_t level, bool copy_mappings)
      -> Expected<void>;

  /**
   * @brief 在页表中查找（或分配）虚拟地址对应的叶层页表项指针
   *
   * 从根页表开始，逐级取出虚拟页号，定位到叶层页表项。
   * 若中间层不存在且 allocate=true，自动分配新页表并插入。
   *
   * @param page_dir     根页表目录
   * @param virtual_addr 目标虚拟地址
   * @param allocate     中间层不存在时是否自动分配
   * @return Expected<uint64_t*> 叶层页表项的指针，
   *                             失败返回 kVmPageNotMapped 或
   * kVmAllocationFailed
   */
  [[nodiscard]] auto FindPageTableEntry(void* page_dir, void* virtual_addr,
                                        bool allocate = false)
      -> Expected<uint64_t*>;
};

#endif /* NAILONG_KERNEL_SRC_MEMORY_INCLUDE_VIRTUAL_MEMORY_HPP_ */
