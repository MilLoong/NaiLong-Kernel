/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include <cpu_io.h>

#include <cstddef>
#include <cstdint>

#include "arch.h"
#include "basic_info.hpp"
#include "kernel.h"
#include "nk_cstdio"
#include "nk_cstring"
#include "nk_libcxx.h"
#include "nk_stdlib.h"
#include "syscall.hpp"
#include "system_test.h"
#include "task_control_block.hpp"
#include "task_manager.hpp"

auto user_task_test() -> bool {
  nk_printf("user_task_test: start\n");

  return true;
}
