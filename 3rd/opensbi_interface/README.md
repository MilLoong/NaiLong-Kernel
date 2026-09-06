![workflow](https://github.com/MRNIU/opensbi_interface/actions/workflows/workflow.yml/badge.svg)
![commit-activity](https://img.shields.io/github/commit-activity/t/MRNIU/opensbi_interface)
![last-commit](https://img.shields.io/github/last-commit/MRNIU/opensbi_interface)
![MIT License](https://img.shields.io/github/license/mashape/apistatus.svg)
[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg)](https://github.com/996icu/996.ICU/blob/master/LICENSE)
[![996.icu](https://img.shields.io/badge/link-996.icu-red.svg)](https://996.icu)

# opensbi_interface

A comprehensive C interface library for OpenSBI (RISC-V Supervisor Binary Interface)

OpenSBI 的完整 C 接口库，为 RISC-V 裸机开发提供标准化的 SBI 调用接口。

**SPEC Version**: [SBI v2.0](https://github.com/riscv-non-isa/riscv-sbi-doc/releases/tag/v2.0)

**中文版本** | [English Version](README_EN.md)

## 特性

- ✅ **完整的 SBI 规范支持**: 实现 SBI v2.0 规范中的所有扩展
- ✅ **类型安全**: 使用结构化的返回值和错误码
- ✅ **零依赖**: 纯 C 实现，无外部依赖
- ✅ **内联汇编**: 高效的 ecall 实现
- ✅ **广泛兼容**: 支持所有主流 SBI 实现 (OpenSBI, RustSBI, BBL 等)
- ✅ **完整文档**: 详细的 API 文档和使用示例

## 支持的 SBI 扩展

### Base Extension (EID #0x10) ✅
- `sbi_get_spec_version()` - 获取 SBI 规范版本
- `sbi_get_impl_id()` - 获取 SBI 实现 ID
- `sbi_get_impl_version()` - 获取 SBI 实现版本
- `sbi_probe_extension()` - 探测 SBI 扩展支持
- `sbi_get_mvendorid()` - 获取机器厂商 ID
- `sbi_get_marchid()` - 获取机器架构 ID
- `sbi_get_mimpid()` - 获取机器实现 ID

### Timer Extension (EID #0x54494D45 "TIME") ✅
- `sbi_set_timer()` - 设置定时器

### IPI Extension (EID #0x735049 "sPI") ✅
- `sbi_send_ipi()` - 发送处理器间中断

### RFENCE Extension (EID #0x52464E43 "RFNC") ✅
- `sbi_remote_fence_i()` - 远程指令缓存刷新
- `sbi_remote_sfence_vma()` - 远程监管者虚拟内存刷新
- `sbi_remote_sfence_vma_asid()` - 带 ASID 的远程虚拟内存刷新
- `sbi_remote_hfence_gvma()` - 远程客户虚拟内存刷新
- `sbi_remote_hfence_gvma_vmid()` - 带 VMID 的远程客户虚拟内存刷新
- `sbi_remote_hfence_vvma()` - 远程虚拟化虚拟内存刷新
- `sbi_remote_hfence_vvma_asid()` - 带 ASID 的远程虚拟化虚拟内存刷新

### Hart State Management Extension (EID #0x48534D "HSM") ✅
- `sbi_hart_start()` - 启动 Hart
- `sbi_hart_stop()` - 停止 Hart
- `sbi_hart_get_status()` - 获取 Hart 状态
- `sbi_hart_suspend()` - 暂停 Hart

### System Reset Extension (EID #0x53525354 "SRST") ✅
- `sbi_system_reset()` - 系统重置

### Performance Monitoring Unit Extension (EID #0x504D55 "PMU") ✅
- `sbi_pmu_num_counters()` - 获取计数器数量
- `sbi_pmu_counter_get_info()` - 获取计数器信息
- `sbi_pmu_counter_config_matching()` - 配置计数器匹配
- `sbi_pmu_counter_start()` - 启动计数器
- `sbi_pmu_counter_stop()` - 停止计数器
- `sbi_pmu_counter_fw_read()` - 读取固件计数器

### Debug Console Extension (EID #0x4442434E "DBCN") ✅
- `sbi_debug_console_write()` - 调试控制台写入
- `sbi_debug_console_read()` - 调试控制台读取
- `sbi_debug_console_write_byte()` - 调试控制台写入字节

### System Suspend Extension (EID #0x53555350 "SUSP") ✅
- `sbi_system_suspend()` - 系统暂停

### Collaborative Processor Performance Control Extension (EID #0x43505043 "CPPC") ✅
- `sbi_cppc_probe()` - 探测 CPPC 支持
- `sbi_cppc_read()` - 读取 CPPC 寄存器
- `sbi_cppc_read_hi()` - 读取 CPPC 寄存器高位
- `sbi_cppc_write()` - 写入 CPPC 寄存器

### Nested Acceleration Extension (EID #0x4E41434C "NACL") ✅
- `sbi_nacl_probe_feature()` - 探测嵌套加速特性
- `sbi_nacl_set_shmem()` - 设置共享内存
- `sbi_nacl_sync_csr()` - 同步 CSR
- `sbi_nacl_sync_hfence()` - 同步 HFENCE
- `sbi_nacl_sync_sret()` - 同步 SRET

### Steal Time Accounting Extension (EID #0x535441 "STA") ✅
- `sbi_sta_set_shmem()` - 设置共享内存

## 快速开始

### 基本用法

```c
#include "opensbi_interface.h"

int main() {
    // 获取 SBI 信息
    struct sbiret ret = sbi_get_spec_version();
    if (ret.error == SBI_SUCCESS) {
        uint32_t major = ret.value >> 24;
        uint32_t minor = ret.value & 0xFFFFFF;
        printf("SBI Spec Version: %d.%d\n", major, minor);
    }

    // 获取实现信息
    ret = sbi_get_impl_id();
    if (ret.error == SBI_SUCCESS) {
        printf("SBI Implementation: %s\n", SBI_IMPL_ID_NAMES[ret.value]);
    }

    // 探测扩展支持
    ret = sbi_probe_extension(SBI_EXT_TIME);
    if (ret.error == SBI_SUCCESS && ret.value == 1) {
        printf("Timer extension is supported\n");
    }

    return 0;
}
```

### 定时器操作

```c
#include "opensbi_interface.h"

void setup_timer() {
    // 设置 1 秒后的定时器中断
    uint64_t current_time;
    asm volatile("rdtime %0" : "=r"(current_time));
    
    struct sbiret ret = sbi_set_timer(current_time + 10000000);  // 假设 10MHz 频率
    if (ret.error == SBI_SUCCESS) {
        printf("Timer set successfully\n");
    }
}
```

### 处理器间中断 (IPI)

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

### Hart 管理

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

### 调试控制台

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
        // 回退到字节写入
        debug_print(str);
    }
}
```

### 系统重置

```c
#include "opensbi_interface.h"

void system_shutdown() {
    struct sbiret ret = sbi_system_reset(SRST_RESET_TYPE_SHUTDOWN, 
                                        SRST_RESET_REASON_NONE);
    // 此函数不应返回
}

void system_reboot() {
    struct sbiret ret = sbi_system_reset(SRST_RESET_TYPE_COLD_REBOOT, 
                                        SRST_RESET_REASON_NONE);
    // 此函数不应返回
}
```

## 错误处理

库定义了标准的 SBI 错误码：

```c
enum {
    SBI_SUCCESS = 0,                    // 成功
    SBI_ERR_FAILED = -1,               // 失败
    SBI_ERR_NOT_SUPPORTED = -2,        // 不支持
    SBI_ERR_INVALID_PARAM = -3,        // 无效参数
    SBI_ERR_DENIED = -4,               // 拒绝访问
    SBI_ERR_INVALID_ADDRESS = -5,      // 无效地址
    SBI_ERR_ALREADY_AVAILABLE = -6,    // 已经可用
    SBI_ERR_ALREADY_STARTED = -7,      // 已经启动
    SBI_ERR_ALREADY_STOPPED = -8,      // 已经停止
    SBI_ERR_NO_SHMEM = -9,             // 无共享内存
};
```

## 支持的 SBI 实现

库支持以下 SBI 实现：

- **OpenSBI** (推荐)
- **RustSBI**  
- **Berkeley Boot Loader (BBL)**
- **Xvisor**
- **KVM**
- **Diosix**
- **Coffer**
- **Xen Project**
- **PolarFire Hart Software Services**

## 编译和集成

### CMake 集成

```cmake
add_subdirectory(path/to/opensbi_interface)
target_link_libraries(your_target opensbi_interface)
```

### 手动编译

```bash
# 编译静态库
gcc -c src/opensbi_interface.c -I src/include -o opensbi_interface.o
ar rcs libopensbi_interface.a opensbi_interface.o

# 链接到你的项目
gcc your_main.c -L. -lopensbi_interface -I src/include -o your_program
```

## 项目结构

```
opensbi_interface/
├── src/
│   ├── include/
│   │   └── opensbi_interface.h    # 主头文件
│   ├── opensbi_interface.c        # 实现文件
│   └── CMakeLists.txt
├── test/
│   ├── main.c                     # 测试示例
│   ├── boot.S                     # 启动汇编
│   ├── link.ld                    # 链接脚本
│   └── fw_jump.elf               # OpenSBI 固件
├── CMakeLists.txt
└── README.md
```

## 测试和验证

项目包含完整的测试示例：

```bash
# 构建测试
mkdir build && cd build
cmake ..
make

# 在 QEMU 中运行测试
qemu-system-riscv64 -machine virt -cpu rv64 -smp 4 \
    -bios test/fw_jump.elf -kernel test/opensbi_test \
    -nographic -serial mon:stdio
```

## 技术细节

### SBI 调用机制

所有 SBI 调用都通过 `ecall` 指令实现：

```c
static struct sbiret ecall(unsigned long arg0, unsigned long arg1,
                          unsigned long arg2, unsigned long arg3,
                          unsigned long arg4, unsigned long arg5,
                          unsigned long fid, unsigned long eid) {
    struct sbiret ret;
    register uintptr_t a0 __asm__("a0") = (uintptr_t)(arg0);
    register uintptr_t a1 __asm__("a1") = (uintptr_t)(arg1);
    // ... 更多寄存器设置
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

### 返回值结构

```c
struct sbiret {
    long error;  // 错误码
    long value;  // 返回值
};
```

## 兼容性

- **RISC-V 架构**: RV32, RV64
- **特权级别**: 监管者模式 (S-mode)
- **编译器**: GCC, Clang
- **SBI 版本**: v0.1 (Legacy), v0.2+, v2.0

## 许可证

MIT License

## 贡献

欢迎提交 Issue 和 Pull Request！

## 相关资源

- [RISC-V SBI Specification](https://github.com/riscv-non-isa/riscv-sbi-doc)
- [OpenSBI](https://github.com/riscv-software-src/opensbi)
- [RISC-V ISA Manual](https://github.com/riscv/riscv-isa-manual)
- [QEMU RISC-V](https://www.qemu.org/docs/master/system/target-riscv.html)
