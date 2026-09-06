![workflow](https://github.com/MRNIU/opensbi_interface/actions/workflows/workflow.yml/badge.svg)
![commit-activity](https://img.shields.io/github/commit-activity/t/MRNIU/opensbi_interface)
![last-commit](https://img.shields.io/github/last-commit/MRNIU/opensbi_interface)
![MIT License](https://img.shields.io/github/license/mashape/apistatus.svg)
[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg)](https://github.com/996icu/996.ICU/blob/master/LICENSE)
[![996.icu](https://img.shields.io/badge/link-996.icu-red.svg)](https://996.icu)

# opensbi_interface

A comprehensive C interface library for OpenSBI (RISC-V Supervisor Binary Interface)

A complete C interface library for OpenSBI, providing standardized SBI call interfaces for RISC-V bare-metal development.

**SPEC Version**: [SBI v2.0](https://github.com/riscv-non-isa/riscv-sbi-doc/releases/tag/v2.0)

[中文版本](README.md) | **English Version**

## Features

- ✅ **Complete SBI Specification Support**: Implements all extensions in SBI v2.0 specification
- ✅ **Type Safety**: Uses structured return values and error codes
- ✅ **Zero Dependencies**: Pure C implementation with no external dependencies
- ✅ **Inline Assembly**: Efficient ecall implementation
- ✅ **Wide Compatibility**: Supports all mainstream SBI implementations (OpenSBI, RustSBI, BBL, etc.)
- ✅ **Complete Documentation**: Detailed API documentation and usage examples

## Supported SBI Extensions

### Base Extension (EID #0x10) ✅
- `sbi_get_spec_version()` - Get SBI specification version
- `sbi_get_impl_id()` - Get SBI implementation ID
- `sbi_get_impl_version()` - Get SBI implementation version
- `sbi_probe_extension()` - Probe SBI extension support
- `sbi_get_mvendorid()` - Get machine vendor ID
- `sbi_get_marchid()` - Get machine architecture ID
- `sbi_get_mimpid()` - Get machine implementation ID

### Timer Extension (EID #0x54494D45 "TIME") ✅
- `sbi_set_timer()` - Set timer

### IPI Extension (EID #0x735049 "sPI") ✅
- `sbi_send_ipi()` - Send inter-processor interrupt

### RFENCE Extension (EID #0x52464E43 "RFNC") ✅
- `sbi_remote_fence_i()` - Remote instruction cache flush
- `sbi_remote_sfence_vma()` - Remote supervisor virtual memory flush
- `sbi_remote_sfence_vma_asid()` - Remote virtual memory flush with ASID
- `sbi_remote_hfence_gvma()` - Remote guest virtual memory flush
- `sbi_remote_hfence_gvma_vmid()` - Remote guest virtual memory flush with VMID
- `sbi_remote_hfence_vvma()` - Remote virtualized virtual memory flush
- `sbi_remote_hfence_vvma_asid()` - Remote virtualized virtual memory flush with ASID

### Hart State Management Extension (EID #0x48534D "HSM") ✅
- `sbi_hart_start()` - Start Hart
- `sbi_hart_stop()` - Stop Hart
- `sbi_hart_get_status()` - Get Hart status
- `sbi_hart_suspend()` - Suspend Hart

### System Reset Extension (EID #0x53525354 "SRST") ✅
- `sbi_system_reset()` - System reset

### Performance Monitoring Unit Extension (EID #0x504D55 "PMU") ✅
- `sbi_pmu_num_counters()` - Get number of counters
- `sbi_pmu_counter_get_info()` - Get counter information
- `sbi_pmu_counter_config_matching()` - Configure counter matching
- `sbi_pmu_counter_start()` - Start counter
- `sbi_pmu_counter_stop()` - Stop counter
- `sbi_pmu_counter_fw_read()` - Read firmware counter

### Debug Console Extension (EID #0x4442434E "DBCN") ✅
- `sbi_debug_console_write()` - Debug console write
- `sbi_debug_console_read()` - Debug console read
- `sbi_debug_console_write_byte()` - Debug console write byte

### System Suspend Extension (EID #0x53555350 "SUSP") ✅
- `sbi_system_suspend()` - System suspend

### Collaborative Processor Performance Control Extension (EID #0x43505043 "CPPC") ✅
- `sbi_cppc_probe()` - Probe CPPC support
- `sbi_cppc_read()` - Read CPPC register
- `sbi_cppc_read_hi()` - Read CPPC register high bits
- `sbi_cppc_write()` - Write CPPC register

### Nested Acceleration Extension (EID #0x4E41434C "NACL") ✅
- `sbi_nacl_probe_feature()` - Probe nested acceleration features
- `sbi_nacl_set_shmem()` - Set shared memory
- `sbi_nacl_sync_csr()` - Synchronize CSR
- `sbi_nacl_sync_hfence()` - Synchronize HFENCE
- `sbi_nacl_sync_sret()` - Synchronize SRET

### Steal Time Accounting Extension (EID #0x535441 "STA") ✅
- `sbi_sta_set_shmem()` - Set shared memory

## Quick Start

### Basic Usage

```c
#include "opensbi_interface.h"

int main() {
    // Get SBI information
    struct sbiret ret = sbi_get_spec_version();
    if (ret.error == SBI_SUCCESS) {
        uint32_t major = ret.value >> 24;
        uint32_t minor = ret.value & 0xFFFFFF;
        printf("SBI Spec Version: %d.%d\n", major, minor);
    }

    // Get implementation information
    ret = sbi_get_impl_id();
    if (ret.error == SBI_SUCCESS) {
        printf("SBI Implementation: %s\n", SBI_IMPL_ID_NAMES[ret.value]);
    }

    // Probe extension support
    ret = sbi_probe_extension(SBI_EXT_TIME);
    if (ret.error == SBI_SUCCESS && ret.value == 1) {
        printf("Timer extension is supported\n");
    }

    return 0;
}
```

### Timer Operations

```c
#include "opensbi_interface.h"

void setup_timer() {
    // Set timer interrupt after 1 second
    uint64_t current_time;
    asm volatile("rdtime %0" : "=r"(current_time));
    
    struct sbiret ret = sbi_set_timer(current_time + 10000000);  // Assuming 10MHz frequency
    if (ret.error == SBI_SUCCESS) {
        printf("Timer set successfully\n");
    }
}
```

### Inter-Processor Interrupts (IPI)

```c
#include "opensbi_interface.h"

void send_ipi_to_hart(unsigned long hart_id) {
    unsigned long hart_mask = 1UL << hart_id;
    struct sbiret ret = sbi_send_ipi(hart_mask, 0);
    
    if (ret.error == SBI_SUCCESS) {
        printf("IPI sent to hart %lu\n", hart_id);
    }
}
```

### Hart Management

```c
#include "opensbi_interface.h"

void start_secondary_hart(unsigned long hart_id, unsigned long start_addr) {
    struct sbiret ret = sbi_hart_start(hart_id, start_addr, 0);
    
    switch (ret.error) {
        case SBI_SUCCESS:
            printf("Hart %lu started successfully\n", hart_id);
            break;
        case SBI_ERR_INVALID_PARAM:
            printf("Invalid hart ID or start address\n");
            break;
        case SBI_ERR_ALREADY_AVAILABLE:
            printf("Hart %lu is already running\n", hart_id);
            break;
        default:
            printf("Failed to start hart %lu: %ld\n", hart_id, ret.error);
    }
}
```

### Debug Console

```c
#include "opensbi_interface.h"

void debug_print(const char* str) {
    while (*str) {
        sbi_debug_console_write_byte(*str);
        str++;
    }
}

void debug_print_string(const char* str) {
    int len = strlen(str);
    unsigned long addr = (unsigned long)str;
    
    struct sbiret ret = sbi_debug_console_write(len, 
                                               addr & 0xFFFFFFFF,
                                               addr >> 32);
    if (ret.error != SBI_SUCCESS) {
        // Fallback to byte writing
        debug_print(str);
    }
}
```

### System Reset

```c
#include "opensbi_interface.h"

void system_shutdown() {
    struct sbiret ret = sbi_system_reset(SRST_RESET_TYPE_SHUTDOWN, 
                                        SRST_RESET_REASON_NONE);
    // This function should not return
}

void system_reboot() {
    struct sbiret ret = sbi_system_reset(SRST_RESET_TYPE_COLD_REBOOT, 
                                        SRST_RESET_REASON_NONE);
    // This function should not return
}
```

## Error Handling

The library defines standard SBI error codes:

```c
enum {
    SBI_SUCCESS = 0,                    // Success
    SBI_ERR_FAILED = -1,               // Failed
    SBI_ERR_NOT_SUPPORTED = -2,        // Not supported
    SBI_ERR_INVALID_PARAM = -3,        // Invalid parameter
    SBI_ERR_DENIED = -4,               // Access denied
    SBI_ERR_INVALID_ADDRESS = -5,      // Invalid address
    SBI_ERR_ALREADY_AVAILABLE = -6,    // Already available
    SBI_ERR_ALREADY_STARTED = -7,      // Already started
    SBI_ERR_ALREADY_STOPPED = -8,      // Already stopped
    SBI_ERR_NO_SHMEM = -9,             // No shared memory
};
```

## Supported SBI Implementations

The library supports the following SBI implementations:

- **OpenSBI** (Recommended)
- **RustSBI**  
- **Berkeley Boot Loader (BBL)**
- **Xvisor**
- **KVM**
- **Diosix**
- **Coffer**
- **Xen Project**
- **PolarFire Hart Software Services**

## Build and Integration

### CMake Integration

```cmake
add_subdirectory(path/to/opensbi_interface)
target_link_libraries(your_target opensbi_interface)
```

### Manual Compilation

```bash
# Compile static library
gcc -c src/opensbi_interface.c -I src/include -o opensbi_interface.o
ar rcs libopensbi_interface.a opensbi_interface.o

# Link to your project
gcc your_main.c -L. -lopensbi_interface -I src/include -o your_program
```

## Project Structure

```
opensbi_interface/
├── src/
│   ├── include/
│   │   └── opensbi_interface.h    # Main header file
│   ├── opensbi_interface.c        # Implementation file
│   └── CMakeLists.txt
├── test/
│   ├── main.c                     # Test example
│   ├── boot.S                     # Boot assembly
│   ├── link.ld                    # Linker script
│   └── fw_jump.elf               # OpenSBI firmware
├── CMakeLists.txt
└── README.md
```

## Testing and Verification

The project includes complete test examples:

```bash
# Build tests
mkdir build && cd build
cmake ..
make

# Run tests in QEMU
qemu-system-riscv64 -machine virt -cpu rv64 -smp 4 \
    -bios test/fw_jump.elf -kernel test/opensbi_test \
    -nographic -serial mon:stdio
```

## Technical Details

### SBI Call Mechanism

All SBI calls are implemented through the `ecall` instruction:

```c
static struct sbiret ecall(unsigned long arg0, unsigned long arg1,
                          unsigned long arg2, unsigned long arg3,
                          unsigned long arg4, unsigned long arg5,
                          unsigned long fid, unsigned long eid) {
    struct sbiret ret;
    register uintptr_t a0 __asm__("a0") = (uintptr_t)(arg0);
    register uintptr_t a1 __asm__("a1") = (uintptr_t)(arg1);
    // ... more register setup
    register uintptr_t a7 __asm__("a7") = (uintptr_t)(eid);

    __asm__("ecall"
            : "+r"(a0), "+r"(a1)
            : "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
            : "memory");
    
    ret.error = a0;
    ret.value = a1;
    return ret;
}
```

### Return Value Structure

```c
struct sbiret {
    long error;  // Error code
    long value;  // Return value
};
```

## Compatibility

- **RISC-V Architecture**: RV32, RV64
- **Privilege Level**: Supervisor mode (S-mode)
- **Compilers**: GCC, Clang
- **SBI Versions**: v0.1 (Legacy), v0.2+, v2.0

## License

MIT License

## Contributing

Issues and Pull Requests are welcome!

## Related Resources

- [RISC-V SBI Specification](https://github.com/riscv-non-isa/riscv-sbi-doc)
- [OpenSBI](https://github.com/riscv-software-src/opensbi)
- [RISC-V ISA Manual](https://github.com/riscv/riscv-isa-manual)
- [QEMU RISC-V](https://www.qemu.org/docs/master/system/target-riscv.html)
