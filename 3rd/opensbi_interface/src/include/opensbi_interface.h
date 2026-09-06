
/**
 * @file opensbi_interface.h
 * @brief opensbi 接口头文件
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

#ifndef OPENSBI_INTERFACE_SRC_INCLUDE_OPENSBI_INTERFACE_H
#define OPENSBI_INTERFACE_SRC_INCLUDE_OPENSBI_INTERFACE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

// Standard SBI Errors
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/binary-encoding.adoc#table_standard_sbi_errors
enum {
  SBI_SUCCESS = 0,
  SBI_ERR_FAILED = -1,
  SBI_ERR_NOT_SUPPORTED = -2,
  SBI_ERR_INVALID_PARAM = -3,
  SBI_ERR_DENIED = -4,
  SBI_ERR_INVALID_ADDRESS = -5,
  SBI_ERR_ALREADY_AVAILABLE = -6,
  SBI_ERR_ALREADY_STARTED = -7,
  SBI_ERR_ALREADY_STOPPED = -8,
  SBI_ERR_NO_SHMEM = -9,
};

// SBI EIDs
#define SBI_EXT_0_1_SET_TIMER 0x0
#define SBI_EXT_0_1_CONSOLE_PUTCHAR 0x1
#define SBI_EXT_0_1_CONSOLE_GETCHAR 0x2
#define SBI_EXT_0_1_CLEAR_IPI 0x3
#define SBI_EXT_0_1_SEND_IPI 0x4
#define SBI_EXT_0_1_REMOTE_FENCE_I 0x5
#define SBI_EXT_0_1_REMOTE_SFENCE_VMA 0x6
#define SBI_EXT_0_1_REMOTE_SFENCE_VMA_ASID 0x7
#define SBI_EXT_0_1_SHUTDOWN 0x8
#define SBI_EXT_BASE 0x10
#define SBI_EXT_TIME 0x54494D45
#define SBI_EXT_IPI 0x735049
#define SBI_EXT_RFENCE 0x52464E43
#define SBI_EXT_HSM 0x48534D
#define SBI_EXT_SRST 0x53525354
#define SBI_EXT_PMU 0x504D55
#define SBI_EXT_DBCN 0x4442434E
#define SBI_EXT_SUSP 0x53555350
#define SBI_EXT_CPPC 0x43505043
#define SBI_EXT_NACL 0x4E41434C
#define SBI_EXT_STA 0x535441

// SBI functions must return a pair of values in a0 and a1, with a0 returning an
// error code.
struct sbiret {
  long error;
  long value;
};

// ----------------------------------------------------------------------------
// Base Extension (EID #0x10)
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-base.adoc

// SBI FIDs
#define SBI_EXT_BASE_GET_SPEC_VERSION 0x0
#define SBI_EXT_BASE_GET_IMP_ID 0x1
#define SBI_EXT_BASE_GET_IMP_VERSION 0x2
#define SBI_EXT_BASE_PROBE_EXT 0x3
#define SBI_EXT_BASE_GET_MVENDORID 0x4
#define SBI_EXT_BASE_GET_MARCHID 0x5
#define SBI_EXT_BASE_GET_MIMPID 0x6

/**
 * @brief Get SBI specification version (FID #0)
 * @return struct sbiret.error SBI_SUCCESS
 * @return struct sbiret.value The minor number of the SBI specification is
 * encoded in the low 24 bits, with the major number encoded in the next 7 bits.
 */
struct sbiret sbi_get_spec_version(void);

// SBI Implementation IDs
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-base.adoc#sbi-implementation-ids
enum {
  SBI_IMPL_ID_BBL = 0,
  SBI_IMPL_ID_OPENSBI = 1,
  SBI_IMPL_ID_XVISOR = 2,
  SBI_IMPL_ID_KVM = 3,
  SBI_IMPL_ID_RUSTSBI = 4,
  SBI_IMPL_ID_DIOSIX = 5,
  SBI_IMPL_ID_COFFER = 6,
  SBI_IMPL_ID_XEN = 7,
  SBI_IMPL_ID_POLARFIRE_HSS = 8,
};

[[maybe_unused]] static const char *SBI_IMPL_ID_NAMES[] = {
    "Berkeley Boot Loader (BBL)",
    "OpenSBI",
    "Xvisor",
    "KVM",
    "RustSBI",
    "Diosix",
    "Coffer",
    "Xen Project",
    "PolarFire Hart Software Services",
};

/**
 * @brief Get SBI implementation ID (FID #1)
 * @return sbiret.error SBI_SUCCESS
 * @return sbiret.value Returns the current SBI implementation ID.
 */
struct sbiret sbi_get_impl_id(void);

/**
 * @brief Get SBI implementation version (FID #2)
 * @return sbiret.error SBI_SUCCESS
 * @return sbiret.value Returns the current SBI implementation version.
 */
struct sbiret sbi_get_impl_version(void);

/**
 * @brief Probe SBI extension (FID #3)
 * @return sbiret.error SBI_SUCCESS
 * @return sbiret.value Returns 0 if the given SBI extension ID (EID) is not
 * available, or 1 if it is available unless defined as any other non-zero value
 * by the implementation.
 */
struct sbiret sbi_probe_extension(long extension_id);

/**
 * @brief Get machine vendor ID (FID #4)
 * @return sbiret.error SBI_SUCCESS
 * @return sbiret.value Return a value that is legal for the `mvendorid` CSR
 * and 0 is always a legal value for this CSR.
 */
struct sbiret sbi_get_mvendorid(void);

/**
 * @brief Get machine architecture ID (FID #5)
 * @return sbiret.error SBI_SUCCESS
 * @return sbiret.value Return a value that is legal for the `marchid` CSR and
 * 0 is always a legal value for this CSR.
 */
struct sbiret sbi_get_marchid(void);

/**
 * @brief Get machine implementation ID (FID #6)
 * @return sbiret.error SBI_SUCCESS
 * @return sbiret.value Return a value that is legal for the `mimpid` CSR and 0
 * is always a legal value for this CSR.
 */
struct sbiret sbi_get_mimpid(void);

#if 0
// ----------------------------------------------------------------------------
// Legacy Extensions (EIDs #0x00 - #0x0F)
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-legacy.adoc

/**
 * @brief Set Timer (EID #0x00)
 * Programs the clock for next event after `stime_value` time. This function
 * also clears the pending timer interrupt bit.
 * @return This SBI call returns 0 upon success or an implementation specific
 * negative error code.
 */
long sbi_set_timer(uint64_t stime_value);

/**
 * @brief Console Putchar (EID #0x01)
 * Write data present in `ch` to debug console.
 * @return This SBI call returns 0 upon success or an implementation specific
 * negative error code.
 */
long sbi_console_putchar(int ch);

/**
 * @brief Console Getchar (EID #0x02)
 * Read a byte from debug console.
 * @return The SBI call returns the byte on success, or -1 for failure.
 */
long sbi_console_getchar(void);

/**
 * @brief Clear IPI (EID #0x03)
 * Clears the pending IPIs if any. The IPI is cleared only in the hart for which
 * this SBI call is invoked. `sbi_clear_ipi()` is deprecated because S-mode code
 * can clear `sip.SSIP` CSR bit directly.
 * @return This SBI call returns 0 if no IPI had been pending, or an
 * implementation specific positive value if an IPI had been pending.
 */
long sbi_clear_ipi(void);

/**
 * @brief Send IPI (EID #0x04)
 * Send an inter-processor interrupt to all the harts defined in hart_mask.
 * Interprocessor interrupts manifest at the receiving harts as Supervisor
 * Software Interrupts.
 * @return This SBI call returns 0 upon success or an implementation specific
 * negative error code.
 */
long sbi_send_ipi(const unsigned long *hart_mask);

/**
 * @brief Remote FENCE.I (EID #0x05)
 * Instructs remote harts to execute `FENCE.I` instruction. The `hart_mask` is
 * same as described in `sbi_send_ipi()`.
 * @return This SBI call returns 0 upon success or an implementation specific
 * negative error code.
 */
long sbi_remote_fence_i(const unsigned long *hart_mask);

/**
 * @brief Remote SFENCE.VMA (EID #0x06)
 * Instructs the remote harts to execute one or more `SFENCE.VMA` instructions,
 * covering the range of virtual addresses between `start` and `start + size`.
 * @return This SBI call returns 0 upon success or an implementation specific
 * negative error code.
 */
long sbi_remote_sfence_vma(const unsigned long *hart_mask, unsigned long start,
                           unsigned long size);

/**
 * @brief Remote SFENCE.VMA with ASID (EID #0x07)
 * Instruct the remote harts to execute one or more `SFENCE.VMA` instructions,
 * covering the range of virtual addresses between `start` and `start + size`.
 * This covers only the given `ASID`.
 * @return This SBI call returns 0 upon success or an implementation specific
 * negative error code.
 */
long sbi_remote_sfence_vma_asid(const unsigned long *hart_mask,
                                unsigned long start, unsigned long size,
                                unsigned long asid);

/**
 * @brief System Shutdown (EID #0x08)
 * Puts all the harts to shutdown state from supervisor point of view.
 * @return This SBI call doesn’t return irrespective whether it succeeds or
 * fails.
 */
void sbi_shutdown(void);
#endif

// ----------------------------------------------------------------------------
// Timer Extension (EID #0x54494D45 "TIME")
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-time.adoc

// SBI FID for TIME extension
#define SBI_EXT_TIME_SET_TIMER 0x0

/**
 * @brief Set Timer (FID #0)
 * @param stime_value absolute time
 * @return sbiret.error NOT FOUND IN SPEC
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_set_timer(uint64_t stime_value);

// ----------------------------------------------------------------------------
// IPI Extension (EID #0x735049 "sPI: s-mode IPI")
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-ipi.adoc

// SBI FID for IPI extension
#define SBI_EXT_IPI_SEND_IPI 0x0

/**
 * @brief Send IPI (FID #0)
 * @param hart_mask a scalar bit-vector containing hartids
 * @param hart_mask_base the starting hartid from which the bit-vector must be
 * computed
 * @return sbiret.error SBI_SUCCESS
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_send_ipi(unsigned long hart_mask,
                           unsigned long hart_mask_base);

// ----------------------------------------------------------------------------
// RFENCE Extension (EID #0x52464E43 "RFNC")
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-rfence.adoc

// SBI FIDs for RFENCE extension
#define SBI_EXT_RFENCE_REMOTE_FENCE_I 0x0
#define SBI_EXT_RFENCE_REMOTE_SFENCE_VMA 0x1
#define SBI_EXT_RFENCE_REMOTE_SFENCE_VMA_ASID 0x2
#define SBI_EXT_RFENCE_REMOTE_HFENCE_GVMA_VMID 0x3
#define SBI_EXT_RFENCE_REMOTE_HFENCE_GVMA 0x4
#define SBI_EXT_RFENCE_REMOTE_HFENCE_VVMA_ASID 0x5
#define SBI_EXT_RFENCE_REMOTE_HFENCE_VVMA 0x6

/**
 * @brief Remote FENCE.I (FID #0)
 * @param hart_mask a scalar bit-vector containing hartids
 * @param hart_mask_base the starting hartid from which the bit-vector must be
 * computed
 * @return sbiret.error SBI_SUCCESS
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_remote_fence_i(unsigned long hart_mask,
                                 unsigned long hart_mask_base);

/**
 * @brief Remote SFENCE.VMA (FID #1)
 * @param hart_mask a scalar bit-vector containing hartids
 * @param hart_mask_base the starting hartid from which the bit-vector must be
 * computed
 * @param start_addr the start address of the range
 * @param size the size of the range
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_ADDRESS
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_remote_sfence_vma(unsigned long hart_mask,
                                    unsigned long hart_mask_base,
                                    unsigned long start_addr,
                                    unsigned long size);

/**
 * @brief Remote SFENCE.VMA with ASID (FID #2)
 * @param hart_mask a scalar bit-vector containing hartids
 * @param hart_mask_base the starting hartid from which the bit-vector must be
 * computed
 * @param start_addr the start address of the range
 * @param size the size of the range
 * @param asid the address space identifier
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_ADDRESS
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_remote_sfence_vma_asid(unsigned long hart_mask,
                                         unsigned long hart_mask_base,
                                         unsigned long start_addr,
                                         unsigned long size,
                                         unsigned long asid);

/**
 * @brief Remote HFENCE.GVMA with VMID (FID #3)
 * @param hart_mask a scalar bit-vector containing hartids
 * @param hart_mask_base the starting hartid from which the bit-vector must be
 * computed
 * @param start_addr the start address of the range
 * @param size the size of the range
 * @param vmid the virtual machine identifier
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_NOT_SUPPORTED,
 * SBI_ERR_INVALID_ADDRESS
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_remote_hfence_gvma_vmid(unsigned long hart_mask,
                                          unsigned long hart_mask_base,
                                          unsigned long start_addr,
                                          unsigned long size,
                                          unsigned long vmid);

/**
 * @brief Remote HFENCE.GVMA (FID #4)
 * @param hart_mask a scalar bit-vector containing hartids
 * @param hart_mask_base the starting hartid from which the bit-vector must be
 * computed
 * @param start_addr the start address of the range
 * @param size the size of the range
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_NOT_SUPPORTED,
 * SBI_ERR_INVALID_ADDRESS
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_remote_hfence_gvma(unsigned long hart_mask,
                                     unsigned long hart_mask_base,
                                     unsigned long start_addr,
                                     unsigned long size);

/**
 * @brief Remote HFENCE.VVMA with ASID (FID #5)
 * @param hart_mask a scalar bit-vector containing hartids
 * @param hart_mask_base the starting hartid from which the bit-vector must be
 * computed
 * @param start_addr the start address of the range
 * @param size the size of the range
 * @param asid the address space identifier
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_NOT_SUPPORTED,
 * SBI_ERR_INVALID_ADDRESS
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_remote_hfence_vvma_asid(unsigned long hart_mask,
                                          unsigned long hart_mask_base,
                                          unsigned long start_addr,
                                          unsigned long size,
                                          unsigned long asid);

/**
 * @brief Remote HFENCE.VVMA (FID #6)
 * @param hart_mask a scalar bit-vector containing hartids
 * @param hart_mask_base the starting hartid from which the bit-vector must be
 * computed
 * @param start_addr the start address of the range
 * @param size the size of the range
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_NOT_SUPPORTED,
 * SBI_ERR_INVALID_ADDRESS
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_remote_hfence_vvma(unsigned long hart_mask,
                                     unsigned long hart_mask_base,
                                     unsigned long start_addr,
                                     unsigned long size);

// ----------------------------------------------------------------------------
// Hart State Management Extension (EID #0x48534D "HSM")
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-hsm.adoc

// SBI FIDs for HSM extension
#define SBI_EXT_HSM_HART_START 0x0
#define SBI_EXT_HSM_HART_STOP 0x1
#define SBI_EXT_HSM_HART_GET_STATUS 0x2
#define SBI_EXT_HSM_HART_SUSPEND 0x3

/**
 * @brief Hart start (FID #0)
 * @param hartid specifies the target hart which is to be started
 * @param start_addr points to a runtime-specified physical address, where the
 * hart can start executing in supervisor-mode
 * @param opaque an XLEN-bit value which will be set in the `a1` register when
 * the hart starts executing at `start_addr`
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_ADDRESS,
 * SBI_ERR_INVALID_PARAM, SBI_ERR_ALREADY_AVAILABLE, SBI_ERR_FAILED
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_hart_start(unsigned long hartid, unsigned long start_addr,
                             unsigned long opaque);

/**
 * @brief Hart stop (FID #1)
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_FAILED
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_hart_stop(void);

// HSM Hart States
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-hsm.adoc#table_hsm_states

enum {
  HSM_HART_STATE_STARTED = 0,
  HSM_HART_STATE_STOPPED = 1,
  HSM_HART_STATE_START_PENDING = 2,
  HSM_HART_STATE_STOP_PENDING = 3,
  HSM_HART_STATE_SUSPENDED = 4,
  HSM_HART_STATE_SUSPEND_PENDING = 5,
  HSM_HART_STATE_RESUME_PENDING = 6,
};

[[maybe_unused]] static const char *HSM_HART_STATES_NAME[] = {
    "STARTED",   "STOPPED",         "START_PENDING",  "STOP_PENDING",
    "SUSPENDED", "SUSPEND_PENDING", "RESUME_PENDING",
};

/**
 * @brief Hart get status (FID #2)
 * @param hartid specifies the target hart for which status is required
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM
 * @return sbiret.value HSM Hart States
 */
struct sbiret sbi_hart_get_status(unsigned long hartid);

// HSM Hart Suspend Types
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-hsm.adoc#table_hsm_hart_suspend_types
#define HSM_SUSP_BASE_MASK 0x7fffffff
#define HSM_SUSP_NON_RET_BIT 0x80000000
#define HSM_SUSP_PLAT_BASE 0x10000000
#define HSM_SUSPEND_RET_DEFAULT 0x00000000
#define HSM_SUSPEND_RET_PLATFORM HSM_SUSP_PLAT_BASE
#define HSM_SUSPEND_RET_LAST HSM_SUSP_BASE_MASK
#define HSM_SUSPEND_NON_RET_DEFAULT HSM_SUSP_NON_RET_BIT
#define HSM_SUSPEND_NON_RET_PLATFORM (HSM_SUSP_NON_RET_BIT | HSM_SUSP_PLAT_BASE)
#define HSM_SUSPEND_NON_RET_LAST (HSM_SUSP_NON_RET_BIT | HSM_SUSP_BASE_MASK)

/**
 * @brief Hart suspend (FID #3)
 * @param suspend_type HSM Hart Suspend Types
 * @param resume_addr points to a runtime-specified physical address, where the
 * hart can resume execution in supervisor-mode after a non-retentive suspend
 * @param opaque an XLEN-bit value which will be set in the `a1` register when
 * the hart resumes execution at `resume_addr` after a non-retentive suspend
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM,
 * SBI_ERR_NOT_SUPPORTED, SBI_ERR_INVALID_ADDRESS, SBI_ERR_FAILED
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_hart_suspend(uint32_t suspend_type, unsigned long resume_addr,
                               unsigned long opaque);

// ----------------------------------------------------------------------------
// System Reset Extension (EID #0x53525354 "SRST")
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-sys-reset.adoc

// SBI FIDs for SRST extension
#define SBI_EXT_SRST_RESET 0x0

// SRST System Reset Types
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-sys-reset.adoc#table_srst_system_reset_types
enum {
  SRST_RESET_TYPE_SHUTDOWN = 0x00000000,
  SRST_RESET_TYPE_COLD_REBOOT = 0x00000001,
  SRST_RESET_TYPE_WARM_REBOOT = 0x00000002,
  // 0x00000003 - 0xEFFFFFFF Reserved for future use
  // 0xF0000000 - 0xFFFFFFFF Vendor or platform specific reset type
};

// SRST System Reset Reasons
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-sys-reset.adoc#table_srst_system_reset_reasons
enum {
  SRST_RESET_REASON_NONE = 0x00000000,
  SRST_RESET_REASON_SYSTEM_FAILURE = 0x00000001,
  // 0x00000002 - 0xDFFFFFFF Reserved for future use
  // 0xE0000000 - 0xEFFFFFFF SBI implementation specific reset reason
  // 0xF0000000 - 0xFFFFFFFF Vendor or platform specific reset reason
};

/**
 * @brief System reset (FID #0)
 * @param reset_type SRST System Reset Types
 * @param reset_reason an optional parameter representing the reason for system
 * reset, SRST System Reset Reasons
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM,
 * SBI_ERR_NOT_SUPPORTED, SBI_ERR_FAILED
 * @return sbiret.value HSM Hart States
 */
struct sbiret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason);

// ----------------------------------------------------------------------------
// Performance Monitoring Unit Extension (EID #0x504D55 "PMU")
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-pmu.adoc

// SBI FIDs for PMU extension
#define SBI_EXT_PMU_NUM_COUNTERS 0x0
#define SBI_EXT_PMU_COUNTER_GET_INFO 0x1
#define SBI_EXT_PMU_COUNTER_CFG_MATCH 0x2
#define SBI_EXT_PMU_COUNTER_START 0x3
#define SBI_EXT_PMU_COUNTER_STOP 0x4
#define SBI_EXT_PMU_COUNTER_FW_READ 0x5
#define SBI_EXT_PMU_COUNTER_FW_READ_HI 0x6
#define SBI_EXT_PMU_SNAPSHOT_SET_SHMEM 0x7

// PMU Hardware Events
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-pmu.adoc#table_pmu_hardware_events
enum {
  SBI_PMU_HW_NO_EVENT = 0,
  SBI_PMU_HW_CPU_CYCLES = 1,
  SBI_PMU_HW_INSTRUCTIONS = 2,
  SBI_PMU_HW_CACHE_REFERENCES = 3,
  SBI_PMU_HW_CACHE_MISSES = 4,
  SBI_PMU_HW_BRANCH_INSTRUCTIONS = 5,
  SBI_PMU_HW_BRANCH_MISSES = 6,
  SBI_PMU_HW_BUS_CYCLES = 7,
  SBI_PMU_HW_STALLED_CYCLES_FRONTEND = 8,
  SBI_PMU_HW_STALLED_CYCLES_BACKEND = 9,
  SBI_PMU_HW_REF_CPU_CYCLES = 10,
};

// Generalized hardware cache events
enum {
  SBI_PMU_HW_CACHE_L1D = 0,
  SBI_PMU_HW_CACHE_L1I = 1,
  SBI_PMU_HW_CACHE_LL = 2,
  SBI_PMU_HW_CACHE_DTLB = 3,
  SBI_PMU_HW_CACHE_ITLB = 4,
  SBI_PMU_HW_CACHE_BPU = 5,
  SBI_PMU_HW_CACHE_NODE = 6,
};

// Generalized hardware cache events
enum {
  SBI_PMU_HW_CACHE_OP_READ = 0,
  SBI_PMU_HW_CACHE_OP_WRITE = 1,
  SBI_PMU_HW_CACHE_OP_PREFETCH = 2,
};

// Generalized hardware cache events
enum {
  SBI_PMU_HW_CACHE_RESULT_ACCESS = 0,
  SBI_PMU_HW_CACHE_RESULT_MISS = 1,
};

// PMU Firmware Events
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-pmu.adoc#table_pmu_firmware_events
enum {
  SBI_PMU_FW_MISALIGNED_LOAD = 0,
  SBI_PMU_FW_MISALIGNED_STORE = 1,
  SBI_PMU_FW_ACCESS_LOAD = 2,
  SBI_PMU_FW_ACCESS_STORE = 3,
  SBI_PMU_FW_ILLEGAL_INSN = 4,
  SBI_PMU_FW_SET_TIMER = 5,
  SBI_PMU_FW_IPI_SENT = 6,
  SBI_PMU_FW_IPI_RECVD = 7,
  SBI_PMU_FW_FENCE_I_SENT = 8,
  SBI_PMU_FW_FENCE_I_RECVD = 9,
  SBI_PMU_FW_SFENCE_VMA_SENT = 10,
  SBI_PMU_FW_SFENCE_VMA_RCVD = 11,
  SBI_PMU_FW_SFENCE_VMA_ASID_SENT = 12,
  SBI_PMU_FW_SFENCE_VMA_ASID_RCVD = 13,
  SBI_PMU_FW_HFENCE_GVMA_SENT = 14,
  SBI_PMU_FW_HFENCE_GVMA_RCVD = 15,
  SBI_PMU_FW_HFENCE_GVMA_VMID_SENT = 16,
  SBI_PMU_FW_HFENCE_GVMA_VMID_RCVD = 17,
  SBI_PMU_FW_HFENCE_VVMA_SENT = 18,
  SBI_PMU_FW_HFENCE_VVMA_RCVD = 19,
  SBI_PMU_FW_HFENCE_VVMA_ASID_SENT = 20,
  SBI_PMU_FW_HFENCE_VVMA_ASID_RCVD = 21,
  SBI_PMU_FW_PLATFORM = 0xFFFF,
};

// SBI PMU event idx type
enum {
  SBI_PMU_EVENT_TYPE_HW = 0x0,
  SBI_PMU_EVENT_TYPE_HW_CACHE = 0x1,
  SBI_PMU_EVENT_TYPE_HW_RAW = 0x2,
  SBI_PMU_EVENT_TYPE_FW = 0xf,
  SBI_PMU_EVENT_TYPE_MAX,
};

// SBI PMU counter type
enum {
  SBI_PMU_CTR_TYPE_HW = 0,
  SBI_PMU_CTR_TYPE_FW,
};

/**
 * @brief Get number of counters (FID #0)
 * @return sbiret.error SBI_SUCCESS
 * @return sbiret.value the number of counters (both hardware and firmware)
 */
struct sbiret sbi_pmu_num_counters();

// counter_info
// counter_info[11:0] = CSR (12bit CSR number)
// counter_info[17:12] = Width (One less than number of bits in CSR)
// counter_info[XLEN-2:18] = Reserved for future use
// counter_info[XLEN-1] = Type (0 = hardware and 1 = firmware)

/**
 * @brief Get details of a counter (FID #1)
 * @param counter_idx the counter index
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM
 * @return sbiret.value the `counter_info` described
 */
struct sbiret sbi_pmu_counter_get_info(unsigned long counter_idx);

// PMU Counter Config Match Flags
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-pmu.adoc#table_pmu_counter_cfg_match_flags

enum {
  SBI_PMU_CFG_FLAG_SKIP_MATCH = (1 << 0),
  SBI_PMU_CFG_FLAG_CLEAR_VALUE = (1 << 1),
  SBI_PMU_CFG_FLAG_AUTO_START = (1 << 2),
  SBI_PMU_CFG_FLAG_SET_VUINH = (1 << 3),
  SBI_PMU_CFG_FLAG_SET_VSINH = (1 << 4),
  SBI_PMU_CFG_FLAG_SET_UINH = (1 << 5),
  SBI_PMU_CFG_FLAG_SET_SINH = (1 << 6),
  SBI_PMU_CFG_FLAG_SET_MINH = (1 << 7),
};

/**
 * @brief Find and configure a matching counter (FID #2)
 * @param counter_idx_base the base counter index, the set of counters whereas
 * @param counter_idx_mask the counter index mask, the set of counters whereas
 * @param config_flags additional counter configuration and filter flags, PMU
 * Counter Config Match Flags
 * @param event_idx the event to be monitored
 * @param event_data any additional event configuration
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM,
 * SBI_ERR_NOT_SUPPORTED
 * @return sbiret.value the `counter_idx`
 */
struct sbiret sbi_pmu_counter_config_matching(unsigned long counter_idx_base,
                                              unsigned long counter_idx_mask,
                                              unsigned long config_flags,
                                              unsigned long event_idx,
                                              uint64_t event_data);

// PMU Counter Start Flags
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-pmu.adoc#table_pmu_counter_start_flags
enum {
  SBI_PMU_START_SET_INIT_VALUE = (1 << 0),
  SBI_PMU_START_FLAG_INIT_SNAPSHOT = (1 << 1),
};

/**
 * @brief Start a set of counters (FID #3)
 * @param counter_idx_base the base counter index, the set of counters whereas
 * @param counter_idx_mask the counter index mask, the set of counters whereas
 * @param start_flags PMU Counter Start Flags
 * @param initial_value specifies the initial value of the counter
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM,
 * SBI_ERR_ALREADY_STARTED, SBI_ERR_NO_SHMEM
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_pmu_counter_start(unsigned long counter_idx_base,
                                    unsigned long counter_idx_mask,
                                    unsigned long start_flags,
                                    uint64_t initial_value);

// PMU Counter Stop Flags
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-pmu.adoc#table_pmu_counter_stop_flags
enum {
  SBI_PMU_STOP_FLAG_RESET = (1 << 0),
  SBI_PMU_STOP_FLAG_TAKE_SNAPSHOT = (1 << 1),

};

/**
 * @brief Stop a set of counters (FID #4)
 * @param counter_idx_base the base counter index, the set of counters whereas
 * @param counter_idx_mask the counter index mask, the set of counters whereas
 * @param stop_flags PMU Counter Stop Flags
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM,
 * SBI_ERR_ALREADY_STOPPED, SBI_ERR_NO_SHMEM
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_pmu_counter_stop(unsigned long counter_idx_base,
                                   unsigned long counter_idx_mask,
                                   unsigned long stop_flags);

/**
 * @brief Read a firmware counter (FID #5)
 * @param counter_idx the counter index
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM
 * @return sbiret.value the current firmware counter value
 */
struct sbiret sbi_pmu_counter_fw_read(unsigned long counter_idx);

/**
 * @brief Read a firmware counter high bits (FID #6)
 * @param counter_idx the counter index
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM
 * @return sbiret.value the upper 32 bits of the current firmware counter value
 */
struct sbiret sbi_pmu_counter_fw_read_hi(unsigned long counter_idx);

/**
 * @brief Set PMU snapshot shared memory (FID #7)
 * @param shmem_phys_lo the lower 32 bits of the physical address of the shared
 * @param shmem_phys_hi the upper 32 bits of the physical address of the shared
 * @param flags MUST BE 0
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_NOT_SUPPORTED,
 * SBI_ERR_INVALID_PARAM, SBI_ERR_INVALID_ADDRESS
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_pmu_snapshot_set_shmem(unsigned long shmem_phys_lo,
                                         unsigned long shmem_phys_hi,
                                         unsigned long flags);

// ----------------------------------------------------------------------------
// Debug Console Extension (EID #0x4442434E "DBCN")
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-debug-console.adoc

// SBI FIDs for DBCN extension
#define SBI_EXT_DEBUG_CONSOLE_WRITE 0x0
#define SBI_EXT_DEBUG_CONSOLE_READ 0x1
#define SBI_EXT_DEBUG_CONSOLE_WRITE_BYTE 0x2

/**
 * @brief Console Write (FID #0)
 * @param num_bytes specifies the number of bytes in the input memory
 * @param base_addr_lo specifies the lower XLEN bits of the input memory
 * physical base address
 * @param base_addr_hi specifies the upper XLEN bits of the input memory
 * physical base address
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM, SBI_ERR_DENIED,
 * SBI_ERR_FAILED
 * @return sbiret.value number of bytes written
 */
struct sbiret sbi_debug_console_write(unsigned long num_bytes,
                                      unsigned long base_addr_lo,
                                      unsigned long base_addr_hi);

/**
 * @brief Console Read (FID #1)
 * @param num_bytes specifies the maximum number of bytes which can be written
 * into the output memory
 * @param base_addr_lo specifies the lower XLEN bits of the output memory
 * physical base address
 * @param base_addr_hi specifies the upper XLEN bits of the output memory
 * physical base address
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM, SBI_ERR_DENIED,
 * SBI_ERR_FAILED
 * @return sbiret.value number of bytes read
 */
struct sbiret sbi_debug_console_read(unsigned long num_bytes,
                                     unsigned long base_addr_lo,
                                     unsigned long base_addr_hi);

/**
 * @brief Console Write Byte (FID #2)
 * @param byte the byte to be written
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_DENIED, SBI_ERR_FAILED
 * @return sbiret.value set to 0
 */
struct sbiret sbi_debug_console_write_byte(uint8_t byte);

// ----------------------------------------------------------------------------
// System Suspend Extension (EID #0x53555350 "SUSP")
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-sys-suspend.adoc

// SBI FID for SUSP extension
#define SBI_EXT_SYSTEM_SUSPEND 0x0

// SUSP System Sleep Types
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-sys-suspend.adoc#table_susp_sleep_types
enum {
  SUSPEND_TO_RAM = 0,
};

/**
 * @brief System Suspend (FID #0)
 * @param sleep_type SUSP System Sleep Types
 * @param resume_addr points to a runtime-specified physical address, where the
 * hart can resume execution in supervisor-mode after a system suspend
 * @param opaque an XLEN-bit value which will be set in the `a1` register when
 * the hart resumes execution at `resume_addr` after a system suspend
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM,
 * SBI_ERR_NOT_SUPPORTED, SBI_ERR_INVALID_ADDRESS, SBI_ERR_DENIED,
 * SBI_ERR_FAILED
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_system_suspend(uint32_t sleep_type, unsigned long resume_addr,
                                 unsigned long opaque);

// ----------------------------------------------------------------------------
// CPPC Extension (EID #0x43505043 "CPPC")
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-cppc.adoc

// SBI FIDs for CPPC extension
#define SBI_EXT_CPPC_PROBE 0x0
#define SBI_EXT_CPPC_READ 0x1
#define SBI_EXT_CPPC_READ_HI 0x2
#define SBI_EXT_CPPC_WRITE 0x3

// CPPC Registers
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-cppc.adoc#table_cppc_registers
enum {
  SBI_CPPC_HIGHEST_PERF = 0x00000000,
  SBI_CPPC_NOMINAL_PERF = 0x00000001,
  SBI_CPPC_LOW_NON_LINEAR_PERF = 0x00000002,
  SBI_CPPC_LOWEST_PERF = 0x00000003,
  SBI_CPPC_GUARANTEED_PERF = 0x00000004,
  SBI_CPPC_DESIRED_PERF = 0x00000005,
  SBI_CPPC_MIN_PERF = 0x00000006,
  SBI_CPPC_MAX_PERF = 0x00000007,
  SBI_CPPC_PERF_REDUC_TOLERANCE = 0x00000008,
  SBI_CPPC_TIME_WINDOW = 0x00000009,
  SBI_CPPC_CTR_WRAP_TIME = 0x0000000A,
  SBI_CPPC_REFERENCE_CTR = 0x0000000B,
  SBI_CPPC_DELIVERED_CTR = 0x0000000C,
  SBI_CPPC_PERF_LIMITED = 0x0000000D,
  SBI_CPPC_ENABLE = 0x0000000E,
  SBI_CPPC_AUTO_SEL_ENABLE = 0x0000000F,
  SBI_CPPC_AUTO_ACT_WINDOW = 0x00000010,
  SBI_CPPC_ENERGY_PERF_PREFERENCE = 0x00000011,
  SBI_CPPC_REFERENCE_PERF = 0x00000012,
  SBI_CPPC_LOWEST_FREQ = 0x00000013,
  SBI_CPPC_NOMINAL_FREQ = 0x00000014,
  SBI_CPPC_TRANSITION_LATENCY = 0x80000000,
};

/**
 * @brief Probe CPPC register (FID #0)
 * @param cppc_reg_id the CPPC register ID
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM, SBI_ERR_FAILED
 * @return sbiret.value the register width, 0 if not implemented
 */
struct sbiret sbi_cppc_probe(uint32_t cppc_reg_id);

/**
 * @brief Read CPPC register (FID #1)
 * @param cppc_reg_id the CPPC register ID
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM,
 * SBI_ERR_NOT_SUPPORTED, SBI_ERR_DENIED, SBI_ERR_FAILED
 * @return sbiret.value the lower 32-bit value of the register
 */
struct sbiret sbi_cppc_read(uint32_t cppc_reg_id);

/**
 * @brief Read CPPC register high bits (FID #2)
 * @param cppc_reg_id the CPPC register ID
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM,
 * SBI_ERR_NOT_SUPPORTED, SBI_ERR_DENIED, SBI_ERR_FAILED
 * @return sbiret.value the upper 32-bit value of the register
 */
struct sbiret sbi_cppc_read_hi(uint32_t cppc_reg_id);

/**
 * @brief Write to CPPC register (FID #3)
 * @param cppc_reg_id the CPPC register ID
 * @param val the value to be written
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM,
 * SBI_ERR_NOT_SUPPORTED, SBI_ERR_DENIED, SBI_ERR_FAILED
 * @return sbiret.value the upper 32-bit value of the register
 */
struct sbiret sbi_cppc_write(uint32_t cppc_reg_id, uint64_t val);

// ----------------------------------------------------------------------------
// Nested Acceleration Extension (EID #0x4E41434C "NACL")
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-nested-acceleration.adoc

// SBI FIDs for NACL extension
#define SBI_EXT_NACL_PROBE_FEATURE 0x0
#define SBI_EXT_NACL_SET_SHMEM 0x1
#define SBI_EXT_NACL_SYNC_CSR 0x2
#define SBI_EXT_NACL_SYNC_HFENCE 0x3
#define SBI_EXT_NACL_SYNC_SRET 0x4

// Nested acceleration features
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-nested-acceleration.adoc#table_nacl_features
enum {
  SBI_NACL_FEAT_SYNC_CSR = 0x00000000,
  SBI_NACL_FEAT_SYNC_HFENCE = 0x00000001,
  SBI_NACL_FEAT_SYNC_SRET = 0x00000002,
  SBI_NACL_FEAT_AUTOSWAP_CSR = 0x00000003,
};

/**
 * @brief Probe nested acceleration feature (FID #0)
 * @param feature_id specifies the nested acceleration feature to probe
 * @return sbiret.error SBI_SUCCESS
 * @return sbiret.value It returns 0 if the given feature_id is not available,
 * or 1 if it is available.
 */
struct sbiret sbi_nacl_probe_feature(uint32_t feature_id);

/**
 * @brief Set nested acceleration shared memory (FID #1)
 * @param shmem_phys_lo specifies the lower XLEN bits of the shared memory
 * physical base address
 * @param shmem_phys_hi specifies the upper XLEN bits of the shared memory
 * physical base address
 * @param flags MUST BE 0
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM,
 * SBI_ERR_INVALID_ADDRESS
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_nacl_set_shmem(unsigned long shmem_phys_lo,
                                 unsigned long shmem_phys_hi,
                                 unsigned long flags);

/**
 * @brief Synchronize shared memory CSRs (FID #2)
 * @param csr_num specifies the set of RISC-V H-extension CSRs to be
 * synchronized
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_NOT_SUPPORTED,
 * SBI_ERR_INVALID_PARAM, SBI_ERR_NO_SHMEM
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_nacl_sync_csr(unsigned long csr_num);

/**
 * @brief Synchronize shared memory HFENCEs (FID #3)
 * @param entry_index specifies the set of nested HFENCE entries to be
 * synchronized
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_NOT_SUPPORTED,
 * SBI_ERR_INVALID_PARAM, SBI_ERR_NO_SHMEM
 * @return sbiret.value NOT FOUND IN SPEC
 */
struct sbiret sbi_nacl_sync_hfence(unsigned long entry_index);

/**
 * @brief Synchronize shared memory and emulate SRET (FID #4)
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_NOT_SUPPORTED, SBI_ERR_NO_SHMEM
 * @return sbiret.value not return upon success
 */
struct sbiret sbi_nacl_sync_sret(void);

// ----------------------------------------------------------------------------
// Steal-time Accounting Extension (EID #0x535441 "STA")
/// @see
/// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-steal-time.adoc

// SBI FID for STA extension
#define SBI_EXT_STA_STEAL_TIME_SET_SHMEM 0x0

/**
 * @brief Set Steal-time Shared Memory Address (FID #0)
 * @param shmem_phys_lo specifies the lower XLEN bits of the shared memory
 * physical base address
 * @param shmem_phys_hi specifies the upper XLEN bits of the shared memory
 * physical base address
 * @param flags MUST BE 0
 * @return sbiret.error SBI_SUCCESS, SBI_ERR_INVALID_PARAM,
 * SBI_ERR_INVALID_ADDRESS, SBI_ERR_FAILED
 * @return sbiret.value set to 0
 */
struct sbiret sbi_steal_time_set_shmem(unsigned long shmem_phys_lo,
                                       unsigned long shmem_phys_hi,
                                       unsigned long flags);

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif  // __cplusplus

#endif  // OPENSBI_INTERFACE_SRC_INCLUDE_OPENSBI_INTERFACE_H
