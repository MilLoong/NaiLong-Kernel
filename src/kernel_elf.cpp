/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

/**
 * @file kernel_elf.cpp
 * @brief KernelElf 实现
 *
 * 对应接口：src/include/kernel_elf.hpp
 * 存放路径：src/kernel_elf.cpp
 */

#include "kernel_elf.hpp"

#include <cstring>

#include "kernel_log.hpp"
#include "nk_cassert"

// ─────────────────────────────────────────────────────────────────
// 构造函数
// ─────────────────────────────────────────────────────────────────

KernelElf::KernelElf(uint64_t elf_addr) {
  nk_assert_msg(elf_addr != 0U, "Invalid elf_addr[0x%lX].\n", elf_addr);

  // 第一步：先只映射 EI_NIDENT 字节，够验证 magic + class 就行
  elf_ = std::span<uint8_t>(reinterpret_cast<uint8_t*>(elf_addr), EI_NIDENT);

  CheckElfIdentity().or_else([](Error err) -> Expected<void> {
    klog::Err("KernelElf NOT valid ELF file: %s\n", err.message());
    while (true) {
      cpu_io::Pause();
    }
    return {};
  });

  // 第二步：读取完整 ELF 文件头
  ehdr_ = *reinterpret_cast<const Elf64_Ehdr*>(elf_.data());

  // 第三步：计算 ELF 镜像实际大小
  // 需要覆盖：ELF 头、程序头表、节头表、所有 section 数据
  size_t max_size = EI_NIDENT;

  if (ehdr_.e_phoff != 0) {
    size_t ph_end = ehdr_.e_phoff + ehdr_.e_phnum * ehdr_.e_phentsize;
    if (ph_end > max_size) {
      max_size = ph_end;
    }
  }

  if (ehdr_.e_shoff != 0) {
    size_t sh_end = ehdr_.e_shoff + ehdr_.e_shnum * ehdr_.e_shentsize;
    if (sh_end > max_size) {
      max_size = sh_end;
    }
    // 遍历所有 section，找出最远的末尾地址
    const auto* shdrs =
        reinterpret_cast<const Elf64_Shdr*>(elf_.data() + ehdr_.e_shoff);
    for (int i = 0; i < ehdr_.e_shnum; ++i) {
      size_t section_end = shdrs[i].sh_offset + shdrs[i].sh_size;
      if (section_end > max_size) {
        max_size = section_end;
      }
    }
  }

  // 用完整大小重新建立 span
  elf_ = std::span<uint8_t>(reinterpret_cast<uint8_t*>(elf_addr), max_size);

  // 第四步：建立程序头表和节头表的 span
  phdr_ = std::span<Elf64_Phdr>(
      reinterpret_cast<Elf64_Phdr*>(elf_.data() + ehdr_.e_phoff),
      ehdr_.e_phnum);

  shdr_ = std::span<Elf64_Shdr>(
      reinterpret_cast<Elf64_Shdr*>(elf_.data() + ehdr_.e_shoff),
      ehdr_.e_shnum);

  // 第五步：在节头表中查找 .symtab 和 .strtab
  // e_shstrndx 是"节名字符串表"所在节的索引
  const auto* shstrtab = reinterpret_cast<const char*>(elf_.data()) +
                         shdr_[ehdr_.e_shstrndx].sh_offset;

  for (auto shdr : shdr_) {
#ifdef NAILONG_KERNEL_DEBUG
    klog::Debug("sh_name: [%s]\n", shstrtab + shdr.sh_name);
#endif
    if (strcmp(shstrtab + shdr.sh_name, ".symtab") == 0) {
      // 符号表：每个条目是一个 Elf64_Sym 结构体
      symtab_ = std::span<Elf64_Sym>(
          reinterpret_cast<Elf64_Sym*>(elf_.data() + shdr.sh_offset),
          shdr.sh_size / sizeof(Elf64_Sym));
    } else if (strcmp(shstrtab + shdr.sh_name, ".strtab") == 0) {
      // 字符串表：符号名的连续字节流，每个名称以 '\0' 结尾
      strtab_ = elf_.data() + shdr.sh_offset;
    }
  }
}

// ─────────────────────────────────────────────────────────────────
// GetElfSize
// ─────────────────────────────────────────────────────────────────

auto KernelElf::GetElfSize() const -> size_t { return elf_.size(); }

// ─────────────────────────────────────────────────────────────────
// CheckElfIdentity
// ─────────────────────────────────────────────────────────────────

auto KernelElf::CheckElfIdentity() const -> Expected<void> {
  return CheckElfMagic().and_then([this]() { return CheckElfClass(); });
}

// ─────────────────────────────────────────────────────────────────
// CheckElfMagic
// ─────────────────────────────────────────────────────────────────

auto KernelElf::CheckElfMagic() const -> Expected<void> {
  if ((elf_[EI_MAG0] != ELFMAG0) || (elf_[EI_MAG1] != ELFMAG1) ||
      (elf_[EI_MAG2] != ELFMAG2) || (elf_[EI_MAG3] != ELFMAG3)) {
    return std::unexpected(Error(ErrorCode::kElfInvalidMagic));
  }
  return {};
}

// ─────────────────────────────────────────────────────────────────
// CheckElfClass
// ─────────────────────────────────────────────────────────────────

auto KernelElf::CheckElfClass() const -> Expected<void> {
  if (elf_[EI_CLASS] == ELFCLASS32) {
    return std::unexpected(Error(ErrorCode::kElfUnsupported32Bit));
  }
  if (elf_[EI_CLASS] != ELFCLASS64) {
    return std::unexpected(Error(ErrorCode::kElfInvalidClass));
  }
  return {};
}
