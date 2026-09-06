/**
 * @copyright Copyright The cpu_io Contributors
 */

#ifndef CPU_IO_INCLUDE_X86_64_APIC_HPP_
#define CPU_IO_INCLUDE_X86_64_APIC_HPP_

#include "msr.h"
#include "regs.hpp"

namespace cpu_io {
namespace msr {

/**
 * @brief APIC 相关 MSR 专用命名空间
 */
namespace apic {

/**
 * @brief 读取 APIC Base 寄存器
 * @return uint64_t APIC Base 寄存器的值
 */
static __always_inline auto ReadBase() -> uint64_t {
  return detail::regs::Msr::Read(cpu_io::msr::apic::kBase);
}

/**
 * @brief 写入 APIC Base 寄存器
 * @param value 要写入的值
 */
static __always_inline void WriteBase(uint64_t value) {
  detail::regs::Msr::Write(msr::kIa32ApicBase, value);
}

/**
 * @brief 检查是否为 BSP (Bootstrap Processor)
 * @return true 如果是 BSP，false 否则
 */
static __always_inline auto IsBsp() -> bool {
  return (ReadBase() & msr::apic::base::kBspBit) != 0;
}

/**
 * @brief 检查 APIC 是否全局启用
 * @return true 如果 APIC 全局启用，false 否则
 */
static __always_inline auto IsGloballyEnabled() -> bool {
  return (ReadBase() & msr::apic::base::kGlobalEnableBit) != 0;
}

/**
 * @brief 检查 x2APIC 模式是否启用
 * @return true 如果 x2APIC 模式启用，false 否则
 */
static __always_inline auto IsX2ApicEnabled() -> bool {
  return (ReadBase() & msr::apic::base::kX2ApicEnableBit) != 0;
}

/**
 * @brief 获取 APIC Base 物理地址
 * @return uint64_t APIC Base 物理地址
 */
static __always_inline auto GetBaseAddress() -> uint64_t {
  return ReadBase() & msr::apic::base::kBaseMask;
}

/**
 * @brief 启用 APIC 全局功能
 */
static __always_inline void EnableGlobally() {
  auto value = ReadBase();
  value |= msr::apic::base::kGlobalEnableBit;
  WriteBase(value);
}

/**
 * @brief 禁用 APIC 全局功能
 */
static __always_inline void DisableGlobally() {
  auto value = ReadBase();
  value &= ~msr::apic::base::kGlobalEnableBit;
  WriteBase(value);
}

/**
 * @brief 启用 x2APIC 模式
 */
static __always_inline void EnableX2Apic() {
  auto value = ReadBase();
  value |=
      msr::apic::base::kX2ApicEnableBit | msr::apic::base::kGlobalEnableBit;
  WriteBase(value);
}

/**
 * @brief 禁用 x2APIC 模式
 */
static __always_inline void DisableX2Apic() {
  auto value = ReadBase();
  value &= ~msr::apic::base::kX2ApicEnableBit;
  WriteBase(value);
}

// x2APIC MSR 寄存器操作函数

/**
 * @brief 读取 x2APIC ID 寄存器
 * @return uint32_t x2APIC ID
 */
static __always_inline auto ReadId() -> uint32_t {
  return static_cast<uint32_t>(detail::regs::Msr::Read(msr::apic::kId));
}

/**
 * @brief 读取 x2APIC 版本寄存器
 * @return uint32_t APIC 版本信息
 */
static __always_inline auto ReadVersion() -> uint32_t {
  return static_cast<uint32_t>(detail::regs::Msr::Read(msr::apic::kVersion));
}

/**
 * @brief 读取任务优先级寄存器 (TPR)
 * @return uint32_t 任务优先级值
 */
static __always_inline auto ReadTpr() -> uint32_t {
  return static_cast<uint32_t>(detail::regs::Msr::Read(msr::apic::kTpr));
}

/**
 * @brief 写入任务优先级寄存器 (TPR)
 * @param value 要设置的任务优先级值
 */
static __always_inline void WriteTpr(uint32_t value) {
  detail::regs::Msr::Write(msr::apic::kTpr, value);
}

/**
 * @brief 写入中断结束寄存器 (EOI)
 * @param value EOI 值（通常为 0）
 */
static __always_inline void WriteEoi(uint32_t value = 0) {
  detail::regs::Msr::Write(msr::apic::kEoi, value);
}

/**
 * @brief 读取虚假中断向量寄存器 (SIVR)
 * @return uint32_t SIVR 值
 */
static __always_inline auto ReadSivr() -> uint32_t {
  return static_cast<uint32_t>(detail::regs::Msr::Read(msr::apic::kSivr));
}

/**
 * @brief 写入虚假中断向量寄存器 (SIVR)
 * @param value 要设置的 SIVR 值
 */
static __always_inline void WriteSivr(uint32_t value) {
  detail::regs::Msr::Write(msr::apic::kSivr, value);
}

/**
 * @brief 读取中断命令寄存器 (ICR)
 * @return uint64_t ICR 值
 */
static __always_inline auto ReadIcr() -> uint64_t {
  return detail::regs::Msr::Read(msr::apic::kIcr);
}

/**
 * @brief 写入中断命令寄存器 (ICR)
 * @param value 要发送的 IPI 命令
 */
static __always_inline void WriteIcr(uint64_t value) {
  detail::regs::Msr::Write(msr::apic::kIcr, value);
}

/**
 * @brief 读取 LVT Timer 寄存器
 * @return uint32_t LVT Timer 寄存器值
 */
static __always_inline auto ReadLvtTimer() -> uint32_t {
  return static_cast<uint32_t>(
      detail::regs::Msr::Read(::cpu_io::msr::apic::kLvtTimer));
}

/**
 * @brief 写入 LVT Timer 寄存器
 * @param value 要设置的 LVT Timer 值
 */
static __always_inline void WriteLvtTimer(uint32_t value) {
  detail::regs::Msr::Write(::cpu_io::msr::apic::kLvtTimer, value);
}

/**
 * @brief 读取 LVT LINT0 寄存器
 * @return uint32_t LVT LINT0 寄存器值
 */
static __always_inline auto ReadLvtLint0() -> uint32_t {
  return static_cast<uint32_t>(
      detail::regs::Msr::Read(::cpu_io::msr::apic::kLvtLint0));
}

/**
 * @brief 写入 LVT LINT0 寄存器
 * @param value 要设置的 LVT LINT0 值
 */
static __always_inline void WriteLvtLint0(uint32_t value) {
  detail::regs::Msr::Write(::cpu_io::msr::apic::kLvtLint0, value);
}

/**
 * @brief 读取 LVT LINT1 寄存器
 * @return uint32_t LVT LINT1 寄存器值
 */
static __always_inline auto ReadLvtLint1() -> uint32_t {
  return static_cast<uint32_t>(
      detail::regs::Msr::Read(::cpu_io::msr::apic::kLvtLint1));
}

/**
 * @brief 写入 LVT LINT1 寄存器
 * @param value 要设置的 LVT LINT1 值
 */
static __always_inline void WriteLvtLint1(uint32_t value) {
  detail::regs::Msr::Write(::cpu_io::msr::apic::kLvtLint1, value);
}

/**
 * @brief 读取 LVT Error 寄存器
 * @return uint32_t LVT Error 寄存器值
 */
static __always_inline auto ReadLvtError() -> uint32_t {
  return static_cast<uint32_t>(
      detail::regs::Msr::Read(::cpu_io::msr::apic::kLvtError));
}

/**
 * @brief 写入 LVT Error 寄存器
 * @param value 要设置的 LVT Error 值
 */
static __always_inline void WriteLvtError(uint32_t value) {
  detail::regs::Msr::Write(::cpu_io::msr::apic::kLvtError, value);
}

/**
 * @brief 读取定时器初始计数寄存器
 * @return uint32_t 定时器初始计数值
 */
static __always_inline auto ReadTimerInitCount() -> uint32_t {
  return static_cast<uint32_t>(
      detail::regs::Msr::Read(::cpu_io::msr::apic::kTimerInitCount));
}

/**
 * @brief 写入定时器初始计数寄存器
 * @param value 要设置的定时器初始计数值
 */
static __always_inline void WriteTimerInitCount(uint32_t value) {
  detail::regs::Msr::Write(::cpu_io::msr::apic::kTimerInitCount, value);
}

/**
 * @brief 读取定时器当前计数寄存器
 * @return uint32_t 定时器当前计数值
 */
static __always_inline auto ReadTimerCurrCount() -> uint32_t {
  return static_cast<uint32_t>(
      detail::regs::Msr::Read(::cpu_io::msr::apic::kTimerCurrCount));
}

/**
 * @brief 读取定时器分频配置寄存器
 * @return uint32_t 定时器分频配置值
 */
static __always_inline auto ReadTimerDivide() -> uint32_t {
  return static_cast<uint32_t>(
      detail::regs::Msr::Read(::cpu_io::msr::apic::kTimerDivide));
}

/**
 * @brief 写入定时器分频配置寄存器
 * @param value 要设置的定时器分频配置值
 */
static __always_inline void WriteTimerDivide(uint32_t value) {
  detail::regs::Msr::Write(::cpu_io::msr::apic::kTimerDivide, value);
}

}  // namespace apic
}  // namespace msr
}  // namespace cpu_io

#endif  // CPU_IO_INCLUDE_X86_64_APIC_HPP_
