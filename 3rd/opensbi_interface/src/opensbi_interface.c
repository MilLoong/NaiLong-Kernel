
/**
 * @file opensbi_interface.c
 * @brief opensbi 接口实现
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2024-03-05
 * @copyright MIT LICENSE
 * https://github.com/MRNIU/opensbi_interface
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2024-03-05<td>Zone.N<td>创建文件
 * </table>
 */

#include "include/opensbi_interface.h"

static struct sbiret ecall(unsigned long _arg0, unsigned long _arg1,
                           unsigned long _arg2, unsigned long _arg3,
                           unsigned long _arg4, unsigned long _arg5,
                           unsigned long _fid, unsigned long _eid) {
  struct sbiret ret;
  register uintptr_t a0 __asm__("a0") = (uintptr_t)(_arg0);
  register uintptr_t a1 __asm__("a1") = (uintptr_t)(_arg1);
  register uintptr_t a2 __asm__("a2") = (uintptr_t)(_arg2);
  register uintptr_t a3 __asm__("a3") = (uintptr_t)(_arg3);
  register uintptr_t a4 __asm__("a4") = (uintptr_t)(_arg4);
  register uintptr_t a5 __asm__("a5") = (uintptr_t)(_arg5);
  register uintptr_t a6 __asm__("a6") = (uintptr_t)(_fid);
  register uintptr_t a7 __asm__("a7") = (uintptr_t)(_eid);

  __asm__("ecall"
          : "+r"(a0), "+r"(a1)
          : "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
          : "memory");
  ret.error = a0;
  ret.value = a1;
  return ret;
}

// ----------------------------------------------------------------------------
// Base Extension (EID #0x10)

struct sbiret sbi_get_spec_version() {
  return ecall(0, 0, 0, 0, 0, 0, SBI_EXT_BASE_GET_SPEC_VERSION, SBI_EXT_BASE);
}

struct sbiret sbi_get_impl_id() {
  return ecall(0, 0, 0, 0, 0, 0, SBI_EXT_BASE_GET_IMP_ID, SBI_EXT_BASE);
}

struct sbiret sbi_get_impl_version() {
  return ecall(0, 0, 0, 0, 0, 0, SBI_EXT_BASE_GET_IMP_VERSION, SBI_EXT_BASE);
}

struct sbiret sbi_probe_extension(long extension_id) {
  return ecall(extension_id, 0, 0, 0, 0, 0, SBI_EXT_BASE_PROBE_EXT,
               SBI_EXT_BASE);
}

struct sbiret sbi_get_mvendorid() {
  return ecall(0, 0, 0, 0, 0, 0, SBI_EXT_BASE_GET_MVENDORID, SBI_EXT_BASE);
}

struct sbiret sbi_get_marchid() {
  return ecall(0, 0, 0, 0, 0, 0, SBI_EXT_BASE_GET_MARCHID, SBI_EXT_BASE);
}

struct sbiret sbi_get_mimpid() {
  return ecall(0, 0, 0, 0, 0, 0, SBI_EXT_BASE_GET_MIMPID, SBI_EXT_BASE);
}

#if 0
// ----------------------------------------------------------------------------
// Legacy Extensions (EIDs #0x00 - #0x0F)

long sbi_set_timer(uint64_t stime_value) {
  return ecall(stime_value, 0, 0, 0, 0, 0, 0, SBI_EXT_0_1_SET_TIMER).error;
}

long sbi_console_putchar(int ch) {
  return ecall(ch, 0, 0, 0, 0, 0, 0, SBI_EXT_0_1_CONSOLE_PUTCHAR).error;
}

long sbi_console_getchar() {
  return ecall(0, 0, 0, 0, 0, 0, 0, SBI_EXT_0_1_CONSOLE_GETCHAR).error;
}

long sbi_clear_ipi() {
  return ecall(0, 0, 0, 0, 0, 0, 0, SBI_EXT_0_1_CLEAR_IPI).error;
}

long sbi_send_ipi(const unsigned long *hart_mask) {
  return ecall((unsigned long)hart_mask, 0, 0, 0, 0, 0, 0, SBI_EXT_0_1_SEND_IPI)
      .error;
}

long sbi_remote_fence_i(const unsigned long *hart_mask) {
  return ecall((unsigned long)hart_mask, 0, 0, 0, 0, 0, 0,
               SBI_EXT_0_1_REMOTE_FENCE_I)
      .error;
}

long sbi_remote_sfence_vma(const unsigned long *hart_mask, unsigned long start,
                           unsigned long size) {
  return ecall((unsigned long)hart_mask, start, size, 0, 0, 0, 0,
               SBI_EXT_0_1_REMOTE_SFENCE_VMA)
      .error;
}

long sbi_remote_sfence_vma_asid(const unsigned long *hart_mask,
                                unsigned long start, unsigned long size,
                                unsigned long asid) {
  return ecall((unsigned long)hart_mask, start, size, asid, 0, 0, 0,
               SBI_EXT_0_1_REMOTE_SFENCE_VMA_ASID)
      .error;
}

void sbi_shutdown() {
  return ecall(0, 0, 0, 0, 0, 0, 0, SBI_EXT_0_1_SHUTDOWN).error;
}
#endif

// ----------------------------------------------------------------------------
// Timer Extension (EID #0x54494D45 "TIME")

struct sbiret sbi_set_timer(uint64_t stime_value) {
  return ecall(stime_value, 0, 0, 0, 0, 0, SBI_EXT_TIME_SET_TIMER,
               SBI_EXT_TIME);
}

// ----------------------------------------------------------------------------
// IPI Extension (EID #0x735049 "sPI: s-mode IPI")

struct sbiret sbi_send_ipi(unsigned long hart_mask,
                           unsigned long hart_mask_base) {
  return ecall(hart_mask, hart_mask_base, 0, 0, 0, 0, SBI_EXT_IPI_SEND_IPI,
               SBI_EXT_IPI);
}

// ----------------------------------------------------------------------------
// RFENCE Extension (EID #0x52464E43 "RFNC")

struct sbiret sbi_remote_fence_i(unsigned long hart_mask,
                                 unsigned long hart_mask_base) {
  return ecall(hart_mask, hart_mask_base, 0, 0, 0, 0,
               SBI_EXT_RFENCE_REMOTE_FENCE_I, SBI_EXT_RFENCE);
}

struct sbiret sbi_remote_sfence_vma(unsigned long hart_mask,
                                    unsigned long hart_mask_base,
                                    unsigned long start_addr,
                                    unsigned long size) {
  return ecall(hart_mask, hart_mask_base, start_addr, size, 0, 0,
               SBI_EXT_RFENCE_REMOTE_SFENCE_VMA, SBI_EXT_RFENCE);
}

struct sbiret sbi_remote_sfence_vma_asid(unsigned long hart_mask,
                                         unsigned long hart_mask_base,
                                         unsigned long start_addr,
                                         unsigned long size,
                                         unsigned long asid) {
  return ecall(hart_mask, hart_mask_base, start_addr, size, asid, 0,
               SBI_EXT_RFENCE_REMOTE_SFENCE_VMA_ASID, SBI_EXT_RFENCE);
}

struct sbiret sbi_remote_hfence_gvma_vmid(unsigned long hart_mask,
                                          unsigned long hart_mask_base,
                                          unsigned long start_addr,
                                          unsigned long size,
                                          unsigned long vmid) {
  return ecall(hart_mask, hart_mask_base, start_addr, size, vmid, 0,
               SBI_EXT_RFENCE_REMOTE_HFENCE_GVMA_VMID, SBI_EXT_RFENCE);
}

struct sbiret sbi_remote_hfence_gvma(unsigned long hart_mask,
                                     unsigned long hart_mask_base,
                                     unsigned long start_addr,
                                     unsigned long size) {
  return ecall(hart_mask, hart_mask_base, start_addr, size, 0, 0,
               SBI_EXT_RFENCE_REMOTE_HFENCE_GVMA, SBI_EXT_RFENCE);
}

struct sbiret sbi_remote_hfence_vvma_asid(unsigned long hart_mask,
                                          unsigned long hart_mask_base,
                                          unsigned long start_addr,
                                          unsigned long size,
                                          unsigned long asid) {
  return ecall(hart_mask, hart_mask_base, start_addr, size, asid, 0,
               SBI_EXT_RFENCE_REMOTE_HFENCE_VVMA_ASID, SBI_EXT_RFENCE);
}

struct sbiret sbi_remote_hfence_vvma(unsigned long hart_mask,
                                     unsigned long hart_mask_base,
                                     unsigned long start_addr,
                                     unsigned long size) {
  return ecall(hart_mask, hart_mask_base, start_addr, size, 0, 0,
               SBI_EXT_RFENCE_REMOTE_HFENCE_VVMA, SBI_EXT_RFENCE);
}

// ----------------------------------------------------------------------------
// Hart State Management Extension (EID #0x48534D "HSM")

struct sbiret sbi_hart_start(unsigned long hartid, unsigned long start_addr,
                             unsigned long opaque) {
  return ecall(hartid, start_addr, opaque, 0, 0, 0, SBI_EXT_HSM_HART_START,
               SBI_EXT_HSM);
}

struct sbiret sbi_hart_stop() {
  return ecall(0, 0, 0, 0, 0, 0, SBI_EXT_HSM_HART_STOP, SBI_EXT_HSM);
}

struct sbiret sbi_hart_get_status(unsigned long hartid) {
  return ecall(hartid, 0, 0, 0, 0, 0, SBI_EXT_HSM_HART_GET_STATUS, SBI_EXT_HSM);
}

struct sbiret sbi_hart_suspend(uint32_t suspend_type, unsigned long resume_addr,
                               unsigned long opaque) {
  return ecall(suspend_type, resume_addr, opaque, 0, 0, 0,
               SBI_EXT_HSM_HART_SUSPEND, SBI_EXT_HSM);
}

// ----------------------------------------------------------------------------
// System Reset Extension (EID #0x53525354 "SRST")

struct sbiret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason) {
  return ecall(reset_type, reset_reason, 0, 0, 0, 0, SBI_EXT_SRST_RESET,
               SBI_EXT_SRST);
}

// ----------------------------------------------------------------------------
// Performance Monitoring Unit Extension (EID #0x504D55 "PMU")

struct sbiret sbi_pmu_num_counters() {
  return ecall(0, 0, 0, 0, 0, 0, SBI_EXT_PMU_NUM_COUNTERS, SBI_EXT_PMU);
}

struct sbiret sbi_pmu_counter_get_info(unsigned long counter_idx) {
  return ecall(counter_idx, 0, 0, 0, 0, 0, SBI_EXT_PMU_COUNTER_GET_INFO,
               SBI_EXT_PMU);
}

struct sbiret sbi_pmu_counter_config_matching(unsigned long counter_idx_base,
                                              unsigned long counter_idx_mask,
                                              unsigned long config_flags,
                                              unsigned long event_idx,
                                              uint64_t event_data) {
  return ecall(counter_idx_base, counter_idx_mask, config_flags, event_idx,
               event_data, 0, SBI_EXT_PMU_COUNTER_CFG_MATCH, SBI_EXT_PMU);
}

struct sbiret sbi_pmu_counter_start(unsigned long counter_idx_base,
                                    unsigned long counter_idx_mask,
                                    unsigned long start_flags,
                                    uint64_t initial_value) {
  return ecall(counter_idx_base, counter_idx_mask, start_flags, initial_value,
               0, 0, SBI_EXT_PMU_COUNTER_START, SBI_EXT_PMU);
}

struct sbiret sbi_pmu_counter_stop(unsigned long counter_idx_base,
                                   unsigned long counter_idx_mask,
                                   unsigned long stop_flags) {
  return ecall(counter_idx_base, counter_idx_mask, stop_flags, 0, 0, 0,
               SBI_EXT_PMU_COUNTER_STOP, SBI_EXT_PMU);
}

struct sbiret sbi_pmu_counter_fw_read(unsigned long counter_idx) {
  return ecall(counter_idx, 0, 0, 0, 0, 0, SBI_EXT_PMU_COUNTER_FW_READ,
               SBI_EXT_PMU);
}

struct sbiret sbi_pmu_counter_fw_read_hi(unsigned long counter_idx) {
  return ecall(counter_idx, 0, 0, 0, 0, 0, SBI_EXT_PMU_COUNTER_FW_READ_HI,
               SBI_EXT_PMU);
}

struct sbiret sbi_pmu_snapshot_set_shmem(unsigned long shmem_phys_lo,
                                         unsigned long shmem_phys_hi,
                                         unsigned long flags) {
  return ecall(shmem_phys_lo, shmem_phys_hi, flags, 0, 0, 0,
               SBI_EXT_PMU_SNAPSHOT_SET_SHMEM, SBI_EXT_PMU);
}

// ----------------------------------------------------------------------------
// Debug Console Extension (EID #0x4442434E "DBCN")

struct sbiret sbi_debug_console_write(unsigned long num_bytes,
                                      unsigned long base_addr_lo,
                                      unsigned long base_addr_hi) {
  return ecall(num_bytes, base_addr_lo, base_addr_hi, 0, 0, 0,
               SBI_EXT_DEBUG_CONSOLE_WRITE, SBI_EXT_DBCN);
}

struct sbiret sbi_debug_console_read(unsigned long num_bytes,
                                     unsigned long base_addr_lo,
                                     unsigned long base_addr_hi) {
  return ecall(num_bytes, base_addr_lo, base_addr_hi, 0, 0, 0,
               SBI_EXT_DEBUG_CONSOLE_READ, SBI_EXT_DBCN);
}

struct sbiret sbi_debug_console_write_byte(uint8_t byte) {
  return ecall(byte, 0, 0, 0, 0, 0, SBI_EXT_DEBUG_CONSOLE_WRITE_BYTE,
               SBI_EXT_DBCN);
}

// ----------------------------------------------------------------------------
// System Suspend Extension (EID #0x53555350 "SUSP")

struct sbiret sbi_system_suspend(uint32_t sleep_type, unsigned long resume_addr,
                                 unsigned long opaque) {
  return ecall(sleep_type, resume_addr, opaque, 0, 0, 0, SBI_EXT_SYSTEM_SUSPEND,
               SBI_EXT_SUSP);
}

// ----------------------------------------------------------------------------
// CPPC Extension (EID #0x43505043 "CPPC")

struct sbiret sbi_cppc_probe(uint32_t cppc_reg_id) {
  return ecall(cppc_reg_id, 0, 0, 0, 0, 0, SBI_EXT_CPPC_PROBE, SBI_EXT_CPPC);
}

struct sbiret sbi_cppc_read(uint32_t cppc_reg_id) {
  return ecall(cppc_reg_id, 0, 0, 0, 0, 0, SBI_EXT_CPPC_READ, SBI_EXT_CPPC);
}

struct sbiret sbi_cppc_read_hi(uint32_t cppc_reg_id) {
  return ecall(cppc_reg_id, 0, 0, 0, 0, 0, SBI_EXT_CPPC_READ_HI, SBI_EXT_CPPC);
}

struct sbiret sbi_cppc_write(uint32_t cppc_reg_id, uint64_t val) {
  return ecall(cppc_reg_id, val, 0, 0, 0, 0, SBI_EXT_CPPC_WRITE, SBI_EXT_CPPC);
}

// ----------------------------------------------------------------------------
// Nested Acceleration Extension (EID #0x4E41434C "NACL")

struct sbiret sbi_nacl_probe_feature(uint32_t feature_id) {
  return ecall(feature_id, 0, 0, 0, 0, 0, SBI_EXT_NACL_PROBE_FEATURE,
               SBI_EXT_NACL);
}

struct sbiret sbi_nacl_set_shmem(unsigned long shmem_phys_lo,
                                 unsigned long shmem_phys_hi,
                                 unsigned long flags) {
  return ecall(shmem_phys_lo, shmem_phys_hi, flags, 0, 0, 0,
               SBI_EXT_NACL_SET_SHMEM, SBI_EXT_NACL);
}

struct sbiret sbi_nacl_sync_csr(unsigned long csr_num) {
  return ecall(csr_num, 0, 0, 0, 0, 0, SBI_EXT_NACL_SYNC_CSR, SBI_EXT_NACL);
}

struct sbiret sbi_nacl_sync_hfence(unsigned long entry_index) {
  return ecall(entry_index, 0, 0, 0, 0, 0, SBI_EXT_NACL_SYNC_HFENCE,
               SBI_EXT_NACL);
}

struct sbiret sbi_nacl_sync_sret() {
  return ecall(0, 0, 0, 0, 0, 0, SBI_EXT_NACL_SYNC_SRET, SBI_EXT_NACL);
}

// ----------------------------------------------------------------------------
// Steal-time Accounting Extension (EID #0x535441 "STA")

struct sbiret sbi_steal_time_set_shmem(unsigned long shmem_phys_lo,
                                       unsigned long shmem_phys_hi,
                                       unsigned long flags) {
  return ecall(shmem_phys_lo, shmem_phys_hi, flags, 0, 0, 0,
               SBI_EXT_STA_STEAL_TIME_SET_SHMEM, SBI_EXT_STA);
}

// ----------------------------------------------------------------------------
