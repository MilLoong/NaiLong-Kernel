# cpu_io

Header only multi-arch CPU register Read/Write library

A cross-architecture CPU register read/write library that provides a unified interface for operating CPU registers and devices across different architectures.

**中文版本** | [English Version](README_EN.md)

## Features

- **Header Only**: No compilation required, just include the header files
- **Multi-Architecture Support**: Supports x86_64, AArch64, RISC-V 64-bit architectures
- **Type Safety**: Uses C++ templates and strong typing to ensure operation safety
- **High Performance**: Inline assembly implementation with zero runtime overhead
- **Unified Interface**: Provides consistent APIs across different architectures

## Supported Architectures

### x86_64
- ✅ General register operations
- ✅ Control registers (CR0, CR2, CR3, CR4, CR8)
- ✅ Model Specific Registers (MSR)
- ✅ Segment registers (CS, SS, DS, ES, FS, GS)
- ✅ System table registers (GDTR, IDTR, LDTR, TR)
- ✅ Flags register (RFLAGS)
- ✅ I/O port operations
- ✅ APIC/x2APIC support
- ✅ CPUID instruction wrapper
- ✅ 8259A PIC control
- ✅ 8253/8254 PIT control
- ✅ Serial port control

### AArch64
- ✅ General register operations (X0-X29)
- ✅ System register operations
- ✅ Exception level management
- ✅ Interrupt control (DAIF)
- ✅ Virtualization support
- ✅ Timer control
- ✅ GIC (Generic Interrupt Controller) support

### RISC-V 64
- ✅ General register operations
- ✅ CSR (Control and Status Register) operations
- ✅ Interrupt and exception handling
- ✅ Virtual memory management
- ✅ Timer support

## Quick Start

### Basic Usage

```cpp
#include <cpu_io.h>

// Interrupt control (cross-architecture unified interface)
cpu_io::EnableInterrupt();   // Enable interrupts
cpu_io::DisableInterrupt();  // Disable interrupts
bool status = cpu_io::GetInterruptStatus();  // Get interrupt status

// Get current CPU core ID
size_t core_id = cpu_io::GetCurrentCoreId();
```

### x86_64 Specific Features

```cpp
// I/O port operations
uint8_t data = cpu_io::In<uint8_t>(0x60);    // Read from port
cpu_io::Out<uint8_t>(0x80, 0xFF);            // Write to port

// Control register operations
cpu_io::Cr0::Set();                          // Set CR0
uint64_t cr3_value = cpu_io::Cr3::Get();     // Read CR3

// RFLAGS operations
cpu_io::Rflags::If::Set();                   // Set interrupt flag
cpu_io::Rflags::If::Clear();                 // Clear interrupt flag

// MSR operations
uint64_t msr_value = cpu_io::Msr::Read(0x1B); // Read MSR
cpu_io::Msr::Write(0x1B, value);             // Write MSR

// APIC operations
bool has_apic = cpu_io::cpuid::HasApic();     // Check APIC support
bool has_x2apic = cpu_io::cpuid::HasX2Apic(); // Check x2APIC support
uint32_t apic_id = cpu_io::cpuid::GetApicId(); // Get APIC ID

// APIC Base register operations
uint64_t apic_base = cpu_io::msr::apic::ReadBase();
bool is_bsp = cpu_io::msr::apic::IsBsp();     // Check if BSP
cpu_io::msr::apic::EnableGlobally();          // Enable APIC globally

// 8259A PIC control
cpu_io::Pic pic(0x20, 0x28);                 // Initialize PIC
cpu_io::Pic::Disable();                      // Disable PIC
```

### AArch64 Specific Features

```cpp
// System register operations
uint64_t current_el = cpu_io::CurrentEL::Get(); // Get current exception level
cpu_io::DAIF::I::Set();                          // Disable IRQ
cpu_io::DAIF::I::Clear();                        // Enable IRQ

// Timer operations
cpu_io::CNTV_CTL_EL0::ENABLE::Set();            // Enable virtual timer
uint64_t freq = cpu_io::CNTFRQ_EL0::Get();      // Get timer frequency
uint64_t count = cpu_io::CNTVCT_EL0::Get();     // Get current count

// GIC operations
cpu_io::ICC_PMR_EL1::Set(0xFF);                 // Set interrupt priority mask
cpu_io::ICC_IGRPEN1_EL1::Enable::Set();         // Enable interrupt group 1
```

### RISC-V Specific Features

```cpp
// CSR operations
cpu_io::Sstatus::Sie::Set();                    // Enable supervisor interrupts
uint64_t sstatus = cpu_io::Sstatus::Get();      // Read sstatus

// Interrupt control
cpu_io::Sie::Stie::Set();                       // Enable timer interrupt
cpu_io::Sie::Seie::Set();                       // Enable external interrupt

// Timer operations
uint64_t time = cpu_io::Time::Get();            // Get current time
cpu_io::Stimecmp::Set(time + 1000000);          // Set timer compare value

// Virtual memory
cpu_io::Satp::Mode::Sv39::Set();                // Set Sv39 paging mode
```

## Architecture-Specific File Structure

```
include/
├── cpu_io.h              # Main header file, auto-selects architecture
├── x86_64/               # x86_64 architecture implementation
│   ├── cpu.hpp           # CPU core functionality
│   ├── io.hpp            # I/O port operations
│   ├── apic.hpp          # APIC/x2APIC support
│   ├── cpuid.hpp         # CPUID instruction wrapper
│   ├── msr.h             # MSR definitions and operations
│   ├── pic.hpp           # 8259A PIC control
│   ├── pit.hpp           # 8253/8254 PIT control
│   ├── serial.hpp        # Serial port control
│   ├── regs.hpp          # Register definitions
│   └── regs/             # Register implementation details
├── aarch64/              # AArch64 architecture implementation
│   ├── cpu.hpp           # CPU core functionality
│   ├── regs.hpp          # Register definitions
│   └── regs/             # Register implementation details
└── riscv64/              # RISC-V 64 architecture implementation
    ├── cpu.hpp           # CPU core functionality
    ├── regs.hpp          # Register definitions
    └── regs/             # Register implementation details
```

## Build Requirements

- **C++17** or higher
- Compiler with inline assembly support
- Architecture-specific compilers:
  - x86_64: GCC/Clang
  - AArch64: AArch64 GCC/Clang
  - RISC-V: RISC-V GCC/Clang

## Usage

### CMake Integration

```cmake
# Add subdirectory
add_subdirectory(path/to/cpu_io)

# Link library
target_link_libraries(your_target INTERFACE cpu_io)
```

### Direct Include

```cpp
#include "path/to/cpu_io/include/cpu_io.h"
```

## Design Principles

1. **Type Safety**: Use strong typing to prevent incorrect register operations
2. **Zero Overhead**: All operations compile to direct assembly instructions
3. **Readability**: Provide intuitive API names and documentation
4. **Portability**: Unified interface with architecture-specific implementations
5. **Modularity**: Each functional module is independent and can be included as needed

## Interrupt Context Structures

### x86_64
```cpp
// Interrupt context without error code
struct InterruptContext {
  uint64_t rip, cs, rflags, rsp, ss;
};

// Interrupt context with error code
struct InterruptContextErrorCode {
  uint32_t error_code;
  uint32_t padding;
  uint64_t rip, cs, rflags, rsp, ss;
};
```

## Important Notes

1. **Privilege Levels**: Most register operations require appropriate privilege levels
2. **Architecture Detection**: The library automatically selects the correct architecture implementation based on compiler macros
3. **Inline Assembly**: All low-level operations use inline assembly to ensure performance
4. **Header Dependencies**: As a header-only library, ensure proper inclusion of required header files

## License

MIT

## Contributing

Issues and Pull Requests are welcome to improve this library.

## Related Documentation

- [Intel® 64 and IA-32 Architectures Software Developer's Manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
- [Arm Architecture Reference Manual Armv8](https://developer.arm.com/documentation/ddi0487/latest)
- [RISC-V Instruction Set Manual](https://riscv.org/technical/specifications/)
