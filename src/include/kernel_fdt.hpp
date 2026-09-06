/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#ifndef NAILONG_KERNEL_SRC_INCLUDE_KERNEL_FDT_HPP_
#define NAILONG_KERNEL_SRC_INCLUDE_KERNEL_FDT_HPP_

#include <libfdt_env.h>
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
#include <libfdt.h>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <utility>

#include "expected.hpp"

/**
 * @brief 设备树（FDT/DTB）解析器
 *
 * 📚 学习笔记：什么是 FDT？
 * ─────────────────────────────────────────────────────────────
 * FDT（Flattened Device Tree，扁平设备树）是描述硬件信息的二进制数据结构。
 * Bootloader（如 U-Boot、OpenSBI）在启动内核前把 DTB 文件加载到内存，
 * 并把其地址通过寄存器传给内核（RISC-V: a1, AArch64: x0）。
 *
 * 内核通过解析 FDT 得知：
 *   - 有几个 CPU 核心
 *   - 物理内存从哪里开始、有多大
 *   - 串口在哪个地址、用哪个中断号
 *   - 中断控制器（GIC/PLIC）的寄存器地址
 *   - 时钟频率等
 *
 * x86_64 不使用 FDT（改用 ACPI/fw_cfg），所以这个类主要在
 * RISC-V 和 AArch64 架构下使用。
 *
 * 前置条件：BasicInfo 中 fdt_addr 已指向有效的 DTB 内存区域。
 */
class KernelFdt {
 public:
  /// FDT 头指针（指向 Bootloader 传入的 DTB 内存）
  fdt_header* fdt_header_;

  /**
   * @brief 构造函数
   *
   * 验证 FDT magic number 和 header 完整性，失败则 panic。
   *
   * @param header DTB 在内存中的物理地址
   */
  explicit KernelFdt(uint64_t header);

  /// @name 默认构造/析构函数
  /// @{
  KernelFdt() = default;
  KernelFdt(const KernelFdt&) = default;
  KernelFdt(KernelFdt&&) = default;
  auto operator=(const KernelFdt&) -> KernelFdt& = default;
  auto operator=(KernelFdt&&) -> KernelFdt& = default;
  ~KernelFdt() = default;
  /// @}

  /**
   * @brief 获取 CPU 核心数量
   *
   * 遍历 FDT 所有节点，统计 device_type="cpu" 的节点数。
   *
   * @return Expected<size_t> 成功返回核心数（≥1），
   *                          失败返回 kFdtNodeNotFound
   */
  [[nodiscard]] auto GetCoreCount() const -> Expected<size_t>;

  /**
   * @brief 验证 PSCI 配置（AArch64 SMP 启动依赖）
   *
   * 检查 /psci 节点存在、method="smc"，且 cpu_on/cpu_off/cpu_suspend
   * 的函数 ID 与 SMC64 标准一致。
   *
   * 📚 PSCI（Power State Coordination Interface）是 ARM 定义的
   *    电源管理接口，SMP 启动时通过 PSCI cpu_on 唤醒 AP 核。
   *
   * @return Expected<void> 成功返回空，失败返回 kFdtPropertyNotFound
   */
  [[nodiscard]] auto CheckPSCI() const -> Expected<void>;

  /**
   * @brief 获取物理内存信息
   *
   * 解析 /memory 节点的 reg 属性。
   *
   * @return Expected<pair<uint64_t, size_t>> 成功返回 {基地址, 大小}，
   *                                          失败返回 kFdtNodeNotFound
   */
  [[nodiscard]] auto GetMemory() const -> Expected<std::pair<uint64_t, size_t>>;

  /**
   * @brief 获取控制台串口信息
   *
   * 从 /chosen 节点读取 stdout-path，解析目标节点的
   * reg（基地址、大小）和 interrupts（中断号）。
   *
   * @return Expected<tuple<uint64_t, size_t, uint32_t>>
   *         成功返回 {基地址, 大小, 中断号}，失败返回错误码
   */
  [[nodiscard]] auto GetSerial() const
      -> Expected<std::tuple<uint64_t, size_t, uint32_t>>;

  /**
   * @brief 获取 CPU 时钟基频
   *
   * 解析 /cpus 节点的 timebase-frequency 属性（RISC-V 用于计算定时器）。
   *
   * @return Expected<uint32_t> 成功返回频率（Hz），
   *                            失败返回 kFdtPropertyNotFound
   */
  [[nodiscard]] auto GetTimebaseFrequency() const -> Expected<uint32_t>;

  /**
   * @brief 获取 GIC v3 中断控制器完整信息
   *
   * 解析 compatible="arm,gic-v3" 节点的 reg 属性，
   * 返回 Distributor 和 Redistributor 的地址和大小。
   *
   * @return Expected<tuple<uint64_t, size_t, uint64_t, size_t>>
   *         成功返回 {dist基址, dist大小, redist基址, redist大小}
   */
  [[nodiscard]] auto GetGIC() const
      -> Expected<std::tuple<uint64_t, size_t, uint64_t, size_t>>;

  /**
   * @brief 获取 GIC Distributor 信息（GetGIC 的便捷封装）
   * @return Expected<pair<uint64_t, size_t>> {基地址, 大小}
   */
  [[nodiscard]] auto GetGicDist() const
      -> Expected<std::pair<uint64_t, size_t>>;

  /**
   * @brief 获取 GIC Redistributor 信息（GetGIC 的便捷封装）
   * @return Expected<pair<uint64_t, size_t>> {基地址, 大小}
   */
  [[nodiscard]] auto GetGicCpu() const -> Expected<std::pair<uint64_t, size_t>>;

  /**
   * @brief 获取 AArch64 设备的 GIC 中断号（INTID）
   *
   * 解析指定 compatible 节点的 interrupts 属性。
   * 目前支持：
   *   - "arm,armv8-timer"：返回 EL1 物理定时器中断号
   *   - "arm,pl011"：返回串口中断号
   *
   * @param compatible 设备的 compatible 字符串
   * @return Expected<uint64_t> 成功返回 INTID，失败返回错误码
   */
  [[nodiscard]] auto GetAarch64Intid(const char* compatible) const
      -> Expected<uint64_t>;

  /**
   * @brief 获取 RISC-V PLIC 中断控制器信息
   *
   * 依次尝试 compatible="sifive,plic-1.0.0" 和 "riscv,plic0"，
   * 解析 reg（基址/大小）、riscv,ndev（中断源数）、
   * interrupts-extended（上下文数）。
   *
   * @return Expected<tuple<uint64_t, uint64_t, uint32_t, uint32_t>>
   *         成功返回 {基址, 大小, ndev, context_count}
   */
  [[nodiscard]] auto GetPlic() const
      -> Expected<std::tuple<uint64_t, uint64_t, uint32_t, uint32_t>>;

 private:
  /// PSCI 标准函数 ID（SMC64 调用约定）
  /// @see https://developer.arm.com/documentation/den0022/fb/?lang=en
  static constexpr uint64_t kPsciCpuOnFuncId = 0xC4000003;
  static constexpr uint64_t kPsciCpuOffFuncId = 0x84000002;
  static constexpr uint64_t kPsciCpuSuspendFuncId = 0xC4000001;

  /**
   * @brief 验证 FDT header 合法性（magic、version、totalsize）
   * @return Expected<void> 失败返回 kFdtInvalidHeader
   */
  [[nodiscard]] auto ValidateFdtHeader() const -> Expected<void>;

  /**
   * @brief 按路径查找 FDT 节点，返回其偏移量
   * @param path 绝对路径，如 "/memory"、"/cpus"
   * @return Expected<int> 节点偏移量，失败返回 kFdtNodeNotFound
   */
  [[nodiscard]] auto FindNode(const char* path) const -> Expected<int>;

  /**
   * @brief 按 compatible 字符串查找第一个匹配节点
   * @param compatible 如 "arm,gic-v3"、"sifive,plic-1.0.0"
   * @return Expected<int> 节点偏移量，失败返回 kFdtNodeNotFound
   */
  [[nodiscard]] auto FindCompatibleNode(const char* compatible) const
      -> Expected<int>;

  /**
   * @brief 按 compatible 查找第一个 status="okay"（或无 status）的节点
   * @param compatible compatible 字符串
   * @return Expected<int> 节点偏移量，失败返回 kFdtNodeNotFound
   */
  [[nodiscard]] auto FindEnabledCompatibleNode(const char* compatible) const
      -> Expected<int>;

  /**
   * @brief 读取节点的 reg 属性，解析为 {基地址, 大小}
   * @param offset 节点偏移量
   * @return Expected<pair<uint64_t, size_t>> 失败返回 kFdtPropertyNotFound
   */
  [[nodiscard]] auto GetRegProperty(int offset) const
      -> Expected<std::pair<uint64_t, size_t>>;

  /**
   * @brief 统计 device_type 匹配的节点数量
   * @param device_type 如 "cpu"
   * @return Expected<size_t> 节点计数，解析失败返回 kFdtParseFailed
   */
  [[nodiscard]] auto CountNodesByDeviceType(const char* device_type) const
      -> Expected<size_t>;

  /**
   * @brief 读取 PSCI 节点的 method 属性
   * @param offset PSCI 节点偏移量
   * @return Expected<const char*> "smc" 或 "hvc"，失败返回 kFdtPropertyNotFound
   */
  [[nodiscard]] auto GetPsciMethod(int offset) const -> Expected<const char*>;

  /**
   * @brief 验证 PSCI 函数 ID 与标准是否一致
   * @param offset PSCI 节点偏移量
   * @return Expected<void> 不一致返回 kFdtPropertyNotFound
   */
  [[nodiscard]] auto ValidatePsciFunctionIds(int offset) const
      -> Expected<void>;
};

#endif /* NAILONG_KERNEL_SRC_INCLUDE_KERNEL_FDT_HPP_ */
