# NaiLong-Kernel

**A learning OS kernel — runnable notes on boot, interrupts, memory, and scheduling.**

> Evolved from [SimpleKernel](https://github.com/Simple-XX/SimpleKernel).

Chinese README is the main document: [README.md](./README.md).

## What this is

NaiLong-Kernel is a runnable kernel sketch (primarily **x86_64 + QEMU + U-Boot**) with an **interface-driven** layout: headers carry the contract (declarations + Doxygen); `.cpp` files implement it; tests check the contract.

## Interfaces, Doxygen, and AI prompts

Header Doxygen is both human documentation and a **prompt** for AI-assisted implementation:

- Responsibilities, pre/post conditions, `@param` / `@return`, optional `@code` examples
- Workflow: read the header → implement (or ask an AI for) the `.cpp` → run tests → compare with the in-tree reference
- Tools: GitHub Copilot, ChatGPT/Claude, Copilot Chat / Cursor — paste or open the header as context

See also [`.github/copilot-instructions.md`](./.github/copilot-instructions.md).

Style sketch: C++23, Google C++ style, `.clang-format` / `.clang-tidy`, Doxygen on public interfaces. Macros use `NAILONG_KERNEL_*`; kernel libc headers use the `nk_` prefix.

## Quick start (x86_64)

```bash
cmake --preset build_x86_64
cd build_x86_64
make NaiLong-Kernel NaiLong-Kernel_gen_fit -j$(nproc)
make run
```

Rebuild the FIT after kernel changes (`NaiLong-Kernel_gen_fit`), or TFTP may serve a stale `boot.fit`.

## Docs

See [doc/README.md](./doc/README.md) (Chinese notes on toolchain, boot, console, interrupts).

## License

See [LICENSE](./LICENSE).
