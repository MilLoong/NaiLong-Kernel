/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_INCLUDE_SINGLETON_HPP_
#define NAILONG_KERNEL_SRC_INCLUDE_SINGLETON_HPP_

// 单例模板类
template <typename T>
class Singleton {
 public:
  /// @name 构造/析构函数
  /// @{
  Singleton() = default;
  Singleton(const Singleton&) = delete;
  Singleton(Singleton&&) = delete;
  auto operator=(const Singleton&) -> Singleton& = delete;
  auto operator=(Singleton&&) -> Singleton& = delete;
  ~Singleton() = default;
  /// @}

  // 获取单例实例的静态方法
  static auto GetInstance() -> T& {
    static T instance;
    return instance;
  }
};

#endif /* NAILONG_KERNEL_SRC_INCLUDE_SINGLETON_HPP_ */
