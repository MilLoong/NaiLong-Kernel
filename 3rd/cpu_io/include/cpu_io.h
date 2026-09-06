/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_CPU_IO_H_
#define CPU_IO_INCLUDE_CPU_IO_H_

#ifdef __x86_64__
#include "x86_64/cpu.hpp"
#elif __riscv
#include "riscv64/cpu.hpp"
#elif __aarch64__
#include "aarch64/cpu.hpp"
#endif

#endif /* CPU_IO_INCLUDE_CPU_IO_H_ */
