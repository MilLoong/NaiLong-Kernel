/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_INCLUDE_KERNEL_ELF_HPP_
#define NAILONG_KERNEL_SRC_INCLUDE_KERNEL_ELF_HPP_

#include <elf.h>

#include <cstddef>
#include <cstdint>
#include <span>

#include "expected.hpp"

/**
 * @brief 内核 ELF 文件解析器
 *
 * 📚 学习笔记：内核为什么要解析自己的 ELF？
 * ─────────────────────────────────────────────────────────────
 * 内核本身就是一个 ELF 可执行文件（vmlinux / NaiLong-Kernel.elf）。
 * Bootloader 把它加载进内存后，ELF 文件的完整内容还留在内存里。
 *
 * 解析自身 ELF 有两个用途：
 *   1. 符号表（.symtab + .strtab）：把函数地址翻译成函数名，
 *      这是 DumpStack() 栈回溯能打印函数名的前提。
 *      否则你只能看到一堆 0xFFFFFFFF8012345 这样的裸地址。
 *
 *   2. 程序头表（.phdr）：描述各段（代码段、数据段）的加载地址，
 *      后续内存管理需要知道内核占用了哪些地址范围。
 *
 * 前置条件：BasicInfo.elf_addr 指向内存中完整的 ELF 镜像。
 * 后置条件：symtab_ 和 strtab_ 指向有效的符号/字符串表数据。
 */
class KernelElf {
 public:
  /// 符号表（.symtab section，每项是一个 Elf64_Sym）
  std::span<Elf64_Sym> symtab_;
  /// 字符串表（.strtab section，符号名称的字节流，以 '\0' 分隔）
  uint8_t* strtab_ = nullptr;

  /**
   * @brief 构造函数：解析内存中的 ELF 镜像
   *
   * 执行步骤：
   *   1. 验证 ELF magic number 和 class（必须是 64-bit ELF）
   *   2. 计算 ELF 文件实际大小（遍历所有 section header）
   *   3. 定位程序头表（phdr_）和节头表（shdr_）
   *   4. 在节头表中查找 .symtab 和 .strtab，设置 symtab_/strtab_
   *
   * @param elf_addr 内存中 ELF 镜像的起始地址（通常等于内核加载地址）
   *
   * 前置条件：elf_addr != 0，且指向完整的 ELF 64-bit 镜像。
   * 失败时：打印错误并 panic（死循环），不会返回无效对象。
   */
  explicit KernelElf(uint64_t elf_addr);

  /// @name 默认构造/析构函数
  /// @{
  KernelElf() = default;
  KernelElf(const KernelElf&) = default;
  KernelElf(KernelElf&&) = default;
  auto operator=(const KernelElf&) -> KernelElf& = default;
  auto operator=(KernelElf&&) -> KernelElf& = default;
  ~KernelElf() = default;
  /// @}

  /**
   * @brief 获取 ELF 文件（镜像）大小
   * @return 字节数
   */
  [[nodiscard]] auto GetElfSize() const -> size_t;

 protected:
  /// ELF 文件原始字节视图（指向内存中的 ELF 镜像，不拥有内存）
  std::span<uint8_t> elf_;
  /// ELF 文件头
  Elf64_Ehdr ehdr_ = {};
  /// 程序头表（描述各 segment 的加载信息）
  std::span<Elf64_Phdr> phdr_;
  /// 节头表（描述各 section 的位置和属性）
  std::span<Elf64_Shdr> shdr_;

  /**
   * @brief 验证 ELF 标识字段（magic + class）
   *
   * 组合调用 CheckElfMagic() 和 CheckElfClass()。
   *
   * @return Expected<void> 失败返回 kElfInvalidMagic 或 kElfInvalidClass
   */
  [[nodiscard]] auto CheckElfIdentity() const -> Expected<void>;

 private:
  /**
   * @brief 验证 ELF magic number（\x7fELF）
   * @return Expected<void> 失败返回 kElfInvalidMagic
   */
  [[nodiscard]] auto CheckElfMagic() const -> Expected<void>;

  /**
   * @brief 验证 ELF class 必须为 ELFCLASS64
   *
   * 32-bit ELF 返回 kElfUnsupported32Bit，其他非法值返回 kElfInvalidClass。
   *
   * @return Expected<void>
   */
  [[nodiscard]] auto CheckElfClass() const -> Expected<void>;
};

#endif /* NAILONG_KERNEL_SRC_INCLUDE_KERNEL_ELF_HPP_ */
