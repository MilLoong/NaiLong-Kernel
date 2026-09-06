# cpu_io

Header only multi-arch CPU register Read/Write library

一个跨架构的 CPU 寄存器读写库，提供统一的接口来操作不同架构的 CPU 寄存器和设备。

**中文版本** | [English Version](README_EN.md)

## 特性

- **Header Only**: 无需编译，直接包含头文件即可使用
- **多架构支持**: 支持 x86_64、AArch64、RISC-V 64 位架构
- **类型安全**: 使用 C++ 模板和强类型确保操作安全
- **高性能**: 内联汇编实现，零运行时开销
- **统一接口**: 为不同架构提供一致的 API

## 支持的架构

### x86_64
- ✅ 通用寄存器操作
- ✅ 控制寄存器 (CR0, CR2, CR3, CR4, CR8)
- ✅ 模型特定寄存器 (MSR)
- ✅ 段寄存器 (CS, SS, DS, ES, FS, GS)
- ✅ 系统表寄存器 (GDTR, IDTR, LDTR, TR)
- ✅ 标志寄存器 (RFLAGS)
- ✅ I/O 端口操作
- ✅ APIC/x2APIC 支持
- ✅ CPUID 指令封装
- ✅ 8259A PIC 控制
- ✅ 8253/8254 PIT 控制
- ✅ 串口控制

### AArch64
- ✅ 通用寄存器操作 (X0-X29)
- ✅ 系统寄存器操作
- ✅ 异常级别管理
- ✅ 中断控制 (DAIF)
- ✅ 虚拟化支持
- ✅ 定时器控制
- ✅ GIC (Generic Interrupt Controller) 支持

### RISC-V 64
- ✅ 通用寄存器操作
- ✅ CSR (Control and Status Register) 操作
- ✅ 中断和异常处理
- ✅ 虚拟内存管理
- ✅ 定时器支持

## 快速开始

### 基本用法

```cpp
#include <cpu_io.h>

// 中断控制 (跨架构统一接口)
cpu_io::EnableInterrupt();   // 启用中断
cpu_io::DisableInterrupt();  // 禁用中断
bool status = cpu_io::GetInterruptStatus();  // 获取中断状态

// 获取当前 CPU 核心 ID
size_t core_id = cpu_io::GetCurrentCoreId();
```

### x86_64 特定功能

```cpp
// I/O 端口操作
uint8_t data = cpu_io::In<uint8_t>(0x60);    // 从端口读取
cpu_io::Out<uint8_t>(0x80, 0xFF);            // 向端口写入

// 控制寄存器操作
cpu_io::Cr0::Set();                          // 设置 CR0
uint64_t cr3_value = cpu_io::Cr3::Get();     // 读取 CR3

// RFLAGS 操作
cpu_io::Rflags::If::Set();                   // 设置中断标志
cpu_io::Rflags::If::Clear();                 // 清除中断标志

// MSR 操作
uint64_t msr_value = cpu_io::Msr::Read(0x1B); // 读取 MSR
cpu_io::Msr::Write(0x1B, value);             // 写入 MSR

// APIC 操作
bool has_apic = cpu_io::cpuid::HasApic();     // 检查 APIC 支持
bool has_x2apic = cpu_io::cpuid::HasX2Apic(); // 检查 x2APIC 支持
uint32_t apic_id = cpu_io::cpuid::GetApicId(); // 获取 APIC ID

// APIC Base 寄存器操作
uint64_t apic_base = cpu_io::msr::apic::ReadBase();
bool is_bsp = cpu_io::msr::apic::IsBsp();     // 检查是否为 BSP
cpu_io::msr::apic::EnableGlobally();          // 启用 APIC

// 8259A PIC 控制
cpu_io::Pic pic(0x20, 0x28);                 // 初始化 PIC
cpu_io::Pic::Disable();                      // 禁用 PIC
```

### AArch64 特定功能

```cpp
// 系统寄存器操作
uint64_t current_el = cpu_io::CurrentEL::Get(); // 获取当前异常级别
cpu_io::DAIF::I::Set();                          // 禁用 IRQ
cpu_io::DAIF::I::Clear();                        // 启用 IRQ

// 定时器操作
cpu_io::CNTV_CTL_EL0::ENABLE::Set();            // 启用虚拟定时器
uint64_t freq = cpu_io::CNTFRQ_EL0::Get();      // 获取定时器频率
uint64_t count = cpu_io::CNTVCT_EL0::Get();     // 获取当前计数

// GIC 操作
cpu_io::ICC_PMR_EL1::Set(0xFF);                 // 设置中断优先级掩码
cpu_io::ICC_IGRPEN1_EL1::Enable::Set();         // 启用中断组1
```

### RISC-V 特定功能

```cpp
// CSR 操作
cpu_io::Sstatus::Sie::Set();                    // 启用监管者中断
uint64_t sstatus = cpu_io::Sstatus::Get();      // 读取 sstatus

// 中断控制
cpu_io::Sie::Stie::Set();                       // 启用定时器中断
cpu_io::Sie::Seie::Set();                       // 启用外部中断

// 定时器操作
uint64_t time = cpu_io::Time::Get();            // 获取当前时间
cpu_io::Stimecmp::Set(time + 1000000);          // 设置定时器比较值

// 虚拟内存
cpu_io::Satp::Mode::Sv39::Set();                // 设置 Sv39 分页模式
```

## 架构特定文件结构

```
include/
├── cpu_io.h              # 主头文件，自动选择架构
├── x86_64/               # x86_64 架构实现
│   ├── cpu.hpp           # CPU 核心功能
│   ├── io.hpp            # I/O 端口操作
│   ├── apic.hpp          # APIC/x2APIC 支持
│   ├── cpuid.hpp         # CPUID 指令封装
│   ├── msr.h             # MSR 定义和操作
│   ├── pic.hpp           # 8259A PIC 控制
│   ├── pit.hpp           # 8253/8254 PIT 控制
│   ├── serial.hpp        # 串口控制
│   ├── regs.hpp          # 寄存器定义
│   └── regs/             # 寄存器实现细节
├── aarch64/              # AArch64 架构实现
│   ├── cpu.hpp           # CPU 核心功能
│   ├── regs.hpp          # 寄存器定义
│   └── regs/             # 寄存器实现细节
└── riscv64/              # RISC-V 64 架构实现
    ├── cpu.hpp           # CPU 核心功能
    ├── regs.hpp          # 寄存器定义
    └── regs/             # 寄存器实现细节
```

## 编译要求

- **C++17** 或更高版本
- 支持内联汇编的编译器
- 架构特定的编译器：
  - x86_64: GCC/Clang
  - AArch64: AArch64 GCC/Clang
  - RISC-V: RISC-V GCC/Clang

## 使用方法

### CMake 集成

```cmake
# 添加子目录
add_subdirectory(path/to/cpu_io)

# 链接库
target_link_libraries(your_target INTERFACE cpu_io)
```

### 直接包含

```cpp
#include "path/to/cpu_io/include/cpu_io.h"
```

## 设计原则

1. **类型安全**: 使用强类型防止错误的寄存器操作
2. **零开销**: 所有操作都编译为直接的汇编指令
3. **可读性**: 提供直观的 API 名称和文档
4. **可移植性**: 统一的接口，架构特定的实现
5. **模块化**: 每个功能模块独立，可按需包含

## 中断上下文结构

### x86_64
```cpp
// 无错误码的中断上下文
struct InterruptContext {
  uint64_t rip, cs, rflags, rsp, ss;
};

// 有错误码的中断上下文
struct InterruptContextErrorCode {
  uint32_t error_code;
  uint32_t padding;
  uint64_t rip, cs, rflags, rsp, ss;
};
```

## 注意事项

1. **特权级别**: 大多数寄存器操作需要适当的特权级别
2. **架构检测**: 库会自动根据编译器宏选择正确的架构实现
3. **内联汇编**: 所有底层操作使用内联汇编，确保性能
4. **头文件依赖**: 作为 header-only 库，确保正确包含所需头文件

## 许可证

MIT

## 贡献

欢迎提交 Issue 和 Pull Request 来改进这个库。

## 相关文档

- [Intel® 64 and IA-32 Architectures Software Developer's Manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
- [Arm Architecture Reference Manual Armv8](https://developer.arm.com/documentation/ddi0487/latest)
- [RISC-V Instruction Set Manual](https://riscv.org/technical/specifications/)
