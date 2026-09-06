# NaiLong-Kernel

**用代码当笔记的操作系统内核学习项目**

> 本项目由 [SimpleKernel](https://github.com/Simple-XX/SimpleKernel) 演进而来。

从启动、中断、内存到调度，边读边改、边跑边记。当前主路径以 **x86_64 + QEMU + U-Boot** 为主，并保留 riscv64 / aarch64 相关骨架。

## 目录

- [这是什么](#这是什么)
- [接口、Doxygen 与 AI 提示](#接口doxygen-与-ai-提示)
- [当前能跑到哪](#当前能跑到哪)
- [快速开始（x86_64）](#快速开始x86_64)
- [目录结构](#目录结构)
- [学习文档](#学习文档)
- [构建宏说明](#构建宏说明)
- [许可证](#许可证)

## 这是什么

NaiLong-Kernel 是一份**可运行的内核草稿 + 学习笔记**，同时采用**接口驱动**写法：

- **头文件是契约**：类声明、纯虚接口、类型定义，以及完整的 Doxygen 文档
- **实现与接口分离**：`.cpp` 按头文件契约落地；可用测试对照是否符合接口
- **文档配合源码**：`doc/` 按子系统拆分（工具链、启动、串口、中断……）

目标不是做成产品级 OS，而是：**改得动、跑得起、记得住**。

## 接口、Doxygen 与 AI 提示

头文件里的 Doxygen 不只是给人看的说明，也是给 AI 的 **prompt**：把职责、前置/后置条件、行为约定写清楚，再让工具按契约生成或补全 `.cpp`。

### Doxygen 应覆盖什么

接口（尤其是 `.h` / `.hpp`）建议写全：

| 内容 | 说明 |
|------|------|
| `@brief` | 这个类型/函数是干什么的 |
| 职责与约束 | 适用场景、线程安全、已知实现 |
| 前置 / 后置条件 | 调用前必须成立什么，返回后保证什么 |
| `@param` / `@return` | 参数与返回值语义 |
| `@code` 示例 | 典型用法（可选但推荐） |

示例（风格与仓库中接口一致）：

```cpp
/**
 * @brief 控制台驱动抽象基类
 *
 * 所有串口/控制台驱动必须实现此接口。
 *
 * @pre  硬件已完成基本初始化（时钟使能、引脚配置）
 * @post 调用 PutChar/GetChar 可进行字符级 I/O
 *
 * 已知实现：Ns16550a（RISC-V/x86_64）、Pl011（AArch64）
 */
class ConsoleDriver {
 public:
  virtual ~ConsoleDriver() = default;
  virtual void PutChar(uint8_t c) const = 0;
  [[nodiscard]] virtual auto GetChar() const -> uint8_t = 0;
  [[nodiscard]] virtual auto TryGetChar() const -> uint8_t = 0;
};
```

部分头文件还会用 `📚 学习笔记` 补充设计动机（例如为什么要抽象层、和 Linux 概念的对应），改实现时请一并保留。

### 推荐工作流

1. **读接口**：打开对应 `.h` / `.hpp`，弄清契约  
2. **按契约实现**：自行编写，或把头文件作为上下文交给 AI 生成 `.cpp`（**接口注释就是 prompt**）  
3. **测试验证**：跑单元 / 系统测试，看行为是否符合契约  
4. **对照参考实现**：不过就对比仓库里已有实现，再改

```shell
cmake --preset build_x86_64
cd build_x86_64 && make unit-test
```

### 与 AI 工具怎么配合

| 场景 | 用法 |
|------|------|
| **GitHub Copilot** | 打开头文件，在对应 `.cpp` 里按契约补全实现 |
| **ChatGPT / Claude** | 把头文件贴进上下文，要求生成完整 `.cpp` |
| **Copilot Chat / Cursor** | 选中接口，让 AI 解释契约或生成实现 |
| **自学** | 先想实现思路，再让 AI 生成，对比差异 |

仓库内还有 [`.github/copilot-instructions.md`](./.github/copilot-instructions.md)，方便 Agent / Copilot 对齐本项目约定。

### 代码风格（简要）

- **语言**：C23 / C++23  
- **风格参考**：[Google C++ Style Guide](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents.html)  
- **格式化**：`.clang-format` / `.clang-tidy`  
- **注释**：Doxygen；接口文件应带完整契约文档  

| 类型 | 风格 | 示例 |
|------|------|------|
| 文件 | 小写下划线 | `kernel_log.hpp` |
| 类 / 结构体 | PascalCase | `TaskManager` |
| 函数 | PascalCase / snake_case | `ArchInit` / `sys_yield` |
| 变量 | snake_case | `per_cpu_data` |
| 宏 | SCREAMING_SNAKE | `NAILONG_KERNEL_DEBUG` |
| 常量 | kCamelCase | `kPageSize` |
| 内核 libc / libc++ 头 | `nk_` 前缀 | `nk_cstdio` |

## 当前能跑到哪

x86_64 完整启动路径（非 minimal boot）已验证到：

1. U-Boot 加载 FIT（`boot.fit`）进入内核
2. `MemoryInit` → 建任务 → `InterruptInit`（APIC 定时器）→ `Schedule`
3. 测试任务：`sys_sleep` / `sys_yield` / `sys_exit` 可跑通（约 1 tick = 1ms，`NAILONG_KERNEL_TICK=1000`）

## 快速开始（x86_64）

依赖：Linux 或 WSL、CMake ≥ 3.27、GCC、QEMU、`tftpd`（或等价 TFTP）、已编译的 U-Boot（构建系统会拉/编第三方）。

```bash
cd /path/to/NaiLong-Kernel
cmake --preset build_x86_64
cd build_x86_64
make NaiLong-Kernel NaiLong-Kernel_gen_fit -j$(nproc)

# 将 build_x86_64/bin 提供给 QEMU 的 TFTP（示例）
# ln -sfn "$(pwd)/bin" /srv/tftp/bin

make run   # 或自行用 qemu-system-x86_64 + u-boot.rom 启动
```

成功时串口大致会看到：`Hello x86_64 ArchInit` → `Memory initialization` → `Created 4 test tasks` → `Starting scheduler` → Task1–4 输出。

改内核后务必重新 `make NaiLong-Kernel_gen_fit`，否则 TFTP 里可能仍是旧的 `boot.fit`。

## 目录结构

```
NaiLong-Kernel/
├── README.md              # 本文件
├── README_ENG.md          # 英文摘要
├── CMakeLists.txt
├── CMakePresets.json
├── cmake/                 # 工具链与工程配置
├── doc/                   # 学习笔记（Markdown）
├── src/
│   ├── arch/              # x86_64 / riscv64 / aarch64
│   ├── driver/            # APIC、串口等
│   ├── task/              # 任务、调度、睡眠
│   ├── include/           # 公共头文件
│   └── main.cpp           # 启动与测试任务
├── tests/                 # 单元 / 系统测试
├── tools/
└── 3rd/                   # 第三方（u-boot、bmalloc、cpu_io 等）
```

## 学习文档

见 [`doc/README.md`](./doc/README.md)。建议顺序：

1. [工具链](./doc/0_工具链.md)
2. [系统启动](./doc/1_系统启动.md)
3. [调试输出](./doc/2_调试输出.md)
4. [中断](./doc/3_中断.md)

接口契约优先看各模块头文件中的 Doxygen；调度等模块里的 `📚 学习笔记` 可作补充阅读。

## 构建宏说明

工程 / make 目标名是 **NaiLong-Kernel**。常用编译宏（与 `CMakePresets.json` 一致）例如：`NAILONG_KERNEL_TICK`、`NAILONG_KERNEL_DEBUG`、`NAILONG_KERNEL_MINIMAL_BOOT`。

## 许可证

见 [LICENSE](./LICENSE)。
