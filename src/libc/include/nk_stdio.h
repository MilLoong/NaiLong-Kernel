/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_LIBC_INCLUDE_NK_STDIO_H_
#define NAILONG_KERNEL_SRC_LIBC_INCLUDE_NK_STDIO_H_

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void (*nk_putchar)(int c, void* ctx);

int nk_printf(const char* format, ...) __attribute__((format(printf, 1, 2)));

int nk_snprintf(char* buffer, size_t bufsz, const char* format, ...)
    __attribute__((format(printf, 3, 4)));

int nk_vsnprintf(char* buffer, size_t bufsz, const char* format, va_list vlist)
    __attribute__((format(printf, 3, 0)));

#ifdef __cplusplus
}
#endif

#endif /* NAILONG_KERNEL_SRC_LIBC_INCLUDE_NK_STDIO_H_ */
