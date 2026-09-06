/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include "nk_iostream"

#include <cstdint>

#include "nk_cstdio"

namespace nk_std {

auto ostream::operator<<(int8_t val) -> ostream& {
  nk_printf("%d", val);
  return *this;
}

auto ostream::operator<<(uint8_t val) -> ostream& {
  nk_printf("%d", val);
  return *this;
}

auto ostream::operator<<(const char* val) -> ostream& {
  nk_printf("%s", val);
  return *this;
}

auto ostream::operator<<(int16_t val) -> ostream& {
  nk_printf("%d", val);
  return *this;
}

auto ostream::operator<<(uint16_t val) -> ostream& {
  nk_printf("%d", val);
  return *this;
}

auto ostream::operator<<(int32_t val) -> ostream& {
  nk_printf("%d", val);
  return *this;
}

auto ostream::operator<<(uint32_t val) -> ostream& {
  nk_printf("%d", val);
  return *this;
}

auto ostream::operator<<(int64_t val) -> ostream& {
  nk_printf("%ld", val);
  return *this;
}

auto ostream::operator<<(uint64_t val) -> ostream& {
  nk_printf("%ld", val);
  return *this;
}

auto ostream::operator<<(ostream& (*manip)(ostream&)) -> ostream& {
  return manip(*this);
}

}  // namespace nk_std
