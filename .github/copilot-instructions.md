# NaiLong-Kernel — Agent / Copilot 说明

## 项目定位

NaiLong-Kernel 是可运行的内核学习项目（主路径：x86_64 + QEMU + U-Boot）。**头文件是接口契约**，Doxygen 写清职责与前置/后置条件；`.cpp` 按契约实现。

## 改代码时

1. **先读相关头文件**，以 Doxygen 契约为准；保留头文件中的 `📚 学习笔记` 与示例。
2. **接口注释即 prompt**：补实现或让 AI 生成 `.cpp` 时，以头文件为上下文，不要丢掉 `@brief` / 前置条件 / 行为约定。
3. **只改需要改的行**，避免整文件覆盖冲掉文档与学习说明。
4. x86_64 改完后：`make NaiLong-Kernel`，并 `make NaiLong-Kernel_gen_fit`，再跑 QEMU。
5. 宏名以仓库为准（如 `NAILONG_KERNEL_TICK`）；改名须同步 CMake preset 与全库引用。

## 验证

串口应能看到调度与测试任务（sleep / yield / exit）。定时器处理里若会 `Schedule`，**EOI 须在调度之前**，否则 APIC 会停摆。可用 `make unit-test` 等目标核对接口行为。
