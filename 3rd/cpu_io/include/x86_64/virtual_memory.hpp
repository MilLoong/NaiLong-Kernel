/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_X86_64_VIRTUAL_MEMORY_HPP_
#define CPU_IO_INCLUDE_X86_64_VIRTUAL_MEMORY_HPP_

#include <cstdint>
#include <cstdlib>

#include "regs.hpp"

namespace cpu_io {

namespace virtual_memory {

/// 页表项位偏移定义
static constexpr uint8_t kValidOffset = 0;
static constexpr uint8_t kWriteOffset = 1;
static constexpr uint8_t kUserOffset = 2;
static constexpr uint8_t kWriteThroughOffset = 3;
static constexpr uint8_t kCacheDisableOffset = 4;
static constexpr uint8_t kAccessedOffset = 5;
static constexpr uint8_t kDirtyOffset = 6;
static constexpr uint8_t kHugePageOffset = 7;
static constexpr uint8_t kGlobalOffset = 8;
// Software defined bits (Available to software in x86_64: bits 9-11)
static constexpr uint8_t kReadOffset = 9;
static constexpr uint8_t kExecOffset = 10;

/// 页表项权限位定义
static constexpr uint64_t kValid = 1ULL << kValidOffset;
static constexpr uint64_t kWrite = 1ULL << kWriteOffset;
static constexpr uint64_t kUser = 1ULL << kUserOffset;
static constexpr uint64_t kWriteThrough = 1ULL << kWriteThroughOffset;
static constexpr uint64_t kCacheDisable = 1ULL << kCacheDisableOffset;
static constexpr uint64_t kAccessed = 1ULL << kAccessedOffset;
static constexpr uint64_t kDirty = 1ULL << kDirtyOffset;
static constexpr uint64_t kHugePage = 1ULL << kHugePageOffset;
static constexpr uint64_t kGlobal = 1ULL << kGlobalOffset;
static constexpr uint64_t kRead = 1ULL << kReadOffset;
static constexpr uint64_t kExec = 1ULL << kExecOffset;

/// 页大小常量 (4KB)
static constexpr size_t kPageSize = 4096;
/// 内核虚拟地址相对物理地址的偏移 (Assuming 0 or handled externally)
static constexpr size_t kKernelOffset = 0x0;

/// 页表项属性位数 (bits 0-11 plus NX bit if used, here we treat 0-11 as main
/// attributes)
static constexpr size_t kPteAttributeBits = 12;
/// 页内偏移位数
static constexpr size_t kPageOffsetBits = 12;
/// 虚拟页号位数
static constexpr size_t kVpnBits = 9;
/// 虚拟页号位掩码
static constexpr size_t kVpnMask = 0x1FF;
/// 页表级数（四级页表）
static constexpr size_t kPageTableLevels = 4;

/**
 * @brief 开启分页
 * 在 x86_64 长模式下分页通常已经开启，此函数主要确保 CR0.PG 置位，
 * 但修改 CR0 需谨慎。此处暂留空或仅用于初始化场景。
 */
static __always_inline void EnablePage() {
  // Current x86_64 setup usually implies paging is enabled.
}

/**
 * @brief 关闭分页
 */
static __always_inline void DisablePage() {
  // Disabling paging in long mode leads to GPF unless in 16/32 bit
  // compatibility.
}

/**
 * @brief 设置页目录 (CR3)
 * @param pgd 要设置的页表物理地址
 */
static __always_inline void SetPageDirectory(uint64_t pgd) {
  detail::regs::cr::Cr3::PageDirectoryBase::Write(
      pgd >> detail::register_info::cr::Cr3Info::PageDirectoryBase::kBitOffset);
}

/**
 * @brief 获取页目录 (CR3)
 * @return uint64_t 页目录物理地址值
 */
static __always_inline auto GetPageDirectory() -> uint64_t {
  return detail::regs::cr::Cr3::PageDirectoryBase::Get()
         << detail::register_info::cr::Cr3Info::PageDirectoryBase::kBitOffset;
}

/**
 * @brief 物理地址转换为页表项
 * @param physical_addr 物理地址
 * @param flags 页表项标志位
 * @return uint64_t 页表项值
 */
static __always_inline auto PhysicalToPageTableEntry(uint64_t physical_addr,
                                                     uint64_t flags)
    -> uint64_t {
  // x86_64 PTE: [Physical Address (12-51)] | [Attributes (0-11)] | [NX (63)]
  // Assuming flags handles 0-11 and potentially NX.
  // We keep software bits 9-11 if they are in flags.
  return (physical_addr & 0x000FFFFFFFFFF000ULL) | (flags & 0xFFF) |
         (flags & (1ULL << 63));
}

/**
 * @brief 页表项转换为物理地址
 * @param pte 页表项
 * @return uint64_t 物理地址
 */
static __always_inline auto PageTableEntryToPhysical(uint64_t pte) -> uint64_t {
  return pte & 0x000FFFFFFFFFF000ULL;
}

/**
 * @brief 检查页表项是否有效
 * @param pte 页表项
 * @return true 页表项有效
 * @return false 页表项无效
 */
static __always_inline auto IsPageTableEntryValid(uint64_t pte) -> bool {
  return (pte & kValid) != 0;
}

/**
 * @brief 获取虚拟地址的页号
 * @param virtual_addr 虚拟地址
 * @param level 页表级别（0-3）
 * @return uint64_t 对应级别的页号
 */
static __always_inline auto GetVirtualPageNumber(uint64_t virtual_addr,
                                                 size_t level) -> uint64_t {
  return (virtual_addr >> (kPageOffsetBits + level * kVpnBits)) & kVpnMask;
}

/**
 * @brief 计算页表级别的偏移量
 * @param level 页表级别
 * @return size_t 偏移量
 */
static __always_inline auto GetPageTableLevelShift(size_t level) -> size_t {
  return kPageOffsetBits + (level * kVpnBits);
}

/**
 * @brief 虚拟地址转物理地址（简单的内核空间映射）
 * @param virtual_addr 虚拟地址
 * @return uint64_t 物理地址
 */
static __always_inline auto VirtualToPhysical(uint64_t virtual_addr)
    -> uint64_t {
  return virtual_addr - kKernelOffset;
}

/**
 * @brief 物理地址转虚拟地址（简单的内核空间映射）
 * @param physical_addr 物理地址
 * @return uint64_t 虚拟地址
 */
static __always_inline auto PhysicalToVirtual(uint64_t physical_addr)
    -> uint64_t {
  return physical_addr + kKernelOffset;
}

/**
 * @brief 获取页面对齐的地址
 * @param addr 地址
 * @return uint64_t 页面对齐的地址
 */
static __always_inline auto PageAlign(uint64_t addr) -> uint64_t {
  return addr & ~(kPageSize - 1);
}

/**
 * @brief 获取页面上对齐的地址
 * @param addr 地址
 * @return uint64_t 页面上对齐的地址
 */
static __always_inline auto PageAlignUp(uint64_t addr) -> uint64_t {
  return (addr + kPageSize - 1) & ~(kPageSize - 1);
}

/**
 * @brief 检查地址是否页面对齐
 * @param addr 地址
 * @return true 地址页面对齐
 * @return false 地址未页面对齐
 */
static __always_inline auto IsPageAligned(uint64_t addr) -> bool {
  return (addr & (kPageSize - 1)) == 0;
}

/**
 * @brief 获取页内偏移
 * @param addr 地址
 * @return uint64_t 页内偏移
 */
static __always_inline auto GetPageOffset(uint64_t addr) -> uint64_t {
  return addr & (kPageSize - 1);
}

/**
 * @brief 创建页表项
 * @param physical_addr 物理地址
 * @param readable 可读标志
 * @param writable 可写标志
 * @param executable 可执行标志
 * @param user_accessible 用户可访问标志
 * @param global 全局标志
 * @return uint64_t 页表项
 */
static __always_inline auto CreatePageTableEntry(
    uint64_t physical_addr, bool readable = true, bool writable = false,
    bool executable = false, bool user_accessible = false, bool global = false)
    -> uint64_t {
  uint64_t flags = kValid;
  if (readable) flags |= kRead;
  if (writable) flags |= kWrite;
  if (executable) flags |= kExec;  // Software bit, also implies NX=0 (default)
  if (user_accessible) flags |= kUser;
  if (global) flags |= kGlobal;

  // On x86_64, bit 63 is NX. If we want to support NX:
  // if (!executable) flags |= (1ULL << 63);
  // However, default behavior without setting NX is executable.
  // Just setting kExec (software bit) allows us to track "intent".

  return PhysicalToPageTableEntry(physical_addr, flags);
}

/**
 * @brief 检查页表项权限
 * @param pte 页表项
 * @param check_read 检查读权限
 * @param check_write 检查写权限
 * @param check_exec 检查执行权限
 * @param check_user 检查用户权限
 * @return true 权限检查通过
 * @return false 权限检查失败
 */
static __always_inline auto CheckPageTableEntryPermissions(
    uint64_t pte, bool check_read = false, bool check_write = false,
    bool check_exec = false, bool check_user = false) -> bool {
  if (!IsPageTableEntryValid(pte)) return false;

  // We check against our software bits for Read/Exec, and hardware bits for
  // Write/User
  if (check_read && !(pte & kRead)) return false;
  if (check_write && !(pte & kWrite)) return false;
  if (check_exec && !(pte & kExec)) return false;
  if (check_user && !(pte & kUser)) return false;

  return true;
}

/**
 * @brief 刷新特定地址的 TLB
 * @param virtual_addr 要刷新的虚拟地址
 * @param asid 地址空间标识符（当前忽略）
 */
static __always_inline void FlushTLBAddress(
    uint64_t virtual_addr, [[maybe_unused]] uint64_t asid = 0) {
  __asm__ volatile("invlpg (%0)" : : "r"(virtual_addr) : "memory");
}

/**
 * @brief 刷新所有TLB条目
 */
static __always_inline void FlushTLBAll() {
  // Reloading CR3 flushes TLB
  unsigned long val;
  __asm__ volatile("mov %%cr3, %0; mov %0, %%cr3" : "=r"(val) : : "memory");
}

/**
 * @brief 计算地址范围包含的页数
 * @param start_addr 起始地址
 * @param end_addr 结束地址
 * @return size_t 页数
 */
static __always_inline auto GetPageCount(uint64_t start_addr, uint64_t end_addr)
    -> size_t {
  uint64_t aligned_start = PageAlign(start_addr);
  uint64_t aligned_end = PageAlignUp(end_addr);
  return (aligned_end - aligned_start) / kPageSize;
}

/**
 * @brief 获取页表中间项权限
 * @return uint8_t 页表中间项权限标志
 * In x86_64, intermediate entries must be Present(Valid).
 * To allow children to be Writable or User-accessible, the intermediate
 * entries must have Write and User bits set.
 */
static __always_inline auto GetTableEntryPermissions() -> uint64_t {
  return kValid | kWrite | kUser | kRead | kExec;
  // Add kRead/kExec to software bits for consistency if we check them?
  // But strictly hardware needs Valid, Write, User.
}

/**
 * @brief 获取内核页表权限
 * @param readable 可读标志
 * @param writable 可写标志
 * @param executable 可执行标志
 * @param global 全局标志
 * @return uint64_t 内核页表权限标志
 */
static __always_inline auto GetKernelPagePermissions(bool readable = true,
                                                     bool writable = true,
                                                     bool executable = true,
                                                     bool global = true)
    -> uint64_t {
  uint64_t flags = kValid;
  if (readable) {
    flags |= kRead;
  }
  if (writable) {
    flags |= kWrite;
  }
  if (executable) {
    flags |= kExec;
  }
  if (global) {
    flags |= kGlobal;
  }

  // No User bit for kernel pages
  return flags;
}

/**
 * @brief 获取用户页表权限
 * @param readable 可读标志
 * @param writable 可写标志
 * @param executable 可执行标志
 * @param global 全局标志
 * @return uint64_t 用户页表权限标志
 */
static __always_inline auto GetUserPagePermissions(bool readable = true,
                                                   bool writable = false,
                                                   bool executable = false,
                                                   bool global = false)
    -> uint64_t {
  // User pages must have User bit
  uint64_t flags = kValid | kUser;
  if (readable) {
    flags |= kRead;
  }
  if (writable) {
    flags |= kWrite;
  }
  if (executable) {
    flags |= kExec;
  }
  if (global) {
    flags |= kGlobal;
  }

  return flags;
}

}  // namespace virtual_memory
}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_VIRTUAL_MEMORY_HPP_
