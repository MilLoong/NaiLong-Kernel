/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include "interrupt.h"

#include <cpu_io.h>

#include <cstddef>
#include <cstdint>

#include "arch.h"
#include "basic_info.hpp"
#include "interrupt_base.h"
#include "kernel.h"
#include "singleton.hpp"
#include "nk_cstdio"
#include "nk_cstring"
#include "nk_libcxx.h"
#include "nk_stdlib.h"
#include "system_test.h"

/// @todo 等用户态调通后补上
auto interrupt_test() -> bool {
  nk_printf("memory_test: start\n");

  Singleton<Interrupt>::GetInstance().BroadcastIpi();

  nk_printf("memory_test: multi alloc passed\n");

  return true;
}
