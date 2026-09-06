/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include <opensbi_interface.h>

#include "nk_cstdio"

namespace {

void console_putchar(int c, [[maybe_unused]] void* ctx) {
  sbi_debug_console_write_byte(c);
}

struct EarlyConsole {
  EarlyConsole() { nk_putchar = console_putchar; }
};

EarlyConsole early_console;

}  // namespace
