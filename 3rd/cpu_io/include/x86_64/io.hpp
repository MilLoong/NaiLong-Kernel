/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_X86_64_IO_HPP_
#define CPU_IO_INCLUDE_X86_64_IO_HPP_

#include <cstdint>
#include <type_traits>

/**
 * @brief x86_64 端口 IO
 * @see 64-ia-32-architectures-software-developer-vol-1-manual Section 16.3 I/O
 * PORT PROGRAMMING
 * @note 端口为 16-bit；GCC 15+ 下须用 %w/%b/%k 限定操作数宽度，
 *       否则 `"dN"(uint32_t)` 可能生成 `out/in` 与 DX 宽度不匹配的汇编。
 */
namespace cpu_io {
/**
 * @brief 端口读
 * @tparam T 要读的类型
 * @param  port 要读数据的端口
 * @return 读取到的数据
 */
template <class T>
static __always_inline auto In(const uint16_t port) -> T {
  T data;
  if constexpr (std::is_same_v<T, uint8_t>) {
    __asm__ volatile("inb %w1, %b0" : "=a"(data) : "Nd"(port));
  } else if constexpr (std::is_same_v<T, uint16_t>) {
    __asm__ volatile("inw %w1, %w0" : "=a"(data) : "Nd"(port));
  } else if constexpr (std::is_same_v<T, uint32_t>) {
    __asm__ volatile("inl %w1, %k0" : "=a"(data) : "Nd"(port));
  } else {
    // T 为不支持的类型
    static_assert(sizeof(T) == 0);
  }
  return data;
}

/**
 * @brief 端口写
 * @tparam T 要写的类型
 * @param  port 要写数据的端口
 * @param  data 要写的数据
 */
template <class T>
static __always_inline void Out(const uint16_t port, const T data) {
  if constexpr (std::is_same_v<T, uint8_t>) {
    __asm__ volatile("outb %b1, %w0" : : "Nd"(port), "a"(data));
  } else if constexpr (std::is_same_v<T, uint16_t>) {
    __asm__ volatile("outw %w1, %w0" : : "Nd"(port), "a"(data));
  } else if constexpr (std::is_same_v<T, uint32_t>) {
    __asm__ volatile("outl %k1, %w0" : : "Nd"(port), "a"(data));
  } else {
    // T 为不支持的类型
    static_assert(sizeof(T) == 0);
  }
}

}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_IO_HPP_
