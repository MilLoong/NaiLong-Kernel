# Copyright The NaiLong-Kernel Contributors

# 设置项目名与版本
PROJECT (unit-test)

ENABLE_LANGUAGE (CXX)

ADD_EXECUTABLE (
    ${PROJECT_NAME}
    nk_libc_test.cpp
    nk_ctype_test.cpp
    nk_string_test.cpp
    kernel_elf_test.cpp
    kernel_fdt_test.cpp
    spinlock_test.cpp
    virtual_memory_test.cpp
    nk_list_test.cpp
    nk_queue_test.cpp
    nk_vector_test.cpp
    nk_priority_queue_test.cpp
    nk_rb_tree_test.cpp
    nk_set_test.cpp
    nk_unordered_map_test.cpp
    task/example_environment_test.cpp
    task/task_scheduling_example_test.cpp
    task/fixtures/task_test_harness.cpp
    mocks/arch.cpp
    mocks/test_environment_state.cpp
    ${CMAKE_SOURCE_DIR}/src/syscall.cpp
    ${CMAKE_SOURCE_DIR}/src/task/block.cpp
    ${CMAKE_SOURCE_DIR}/src/task/clone.cpp
    ${CMAKE_SOURCE_DIR}/src/task/exit.cpp
    ${CMAKE_SOURCE_DIR}/src/task/schedule.cpp
    ${CMAKE_SOURCE_DIR}/src/task/sleep.cpp
    ${CMAKE_SOURCE_DIR}/src/task/task_control_block.cpp
    ${CMAKE_SOURCE_DIR}/src/task/task_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/task/tick_update.cpp
    ${CMAKE_SOURCE_DIR}/src/task/wait.cpp
    ${CMAKE_SOURCE_DIR}/src/task/wakeup.cpp
    # 接口重构后新增的实现文件
    ${CMAKE_SOURCE_DIR}/src/kernel_elf.cpp
    ${CMAKE_SOURCE_DIR}/src/kernel_fdt.cpp
    ${CMAKE_SOURCE_DIR}/src/virtual_memory.cpp
    ${CMAKE_SOURCE_DIR}/src/task/mutex.cpp
    ${CMAKE_SOURCE_DIR}/src/task/cfs_scheduler.cpp
    ${CMAKE_SOURCE_DIR}/src/task/fifo_scheduler.cpp
    ${CMAKE_SOURCE_DIR}/src/task/rr_scheduler.cpp
    ${CMAKE_SOURCE_DIR}/src/task/idle_scheduler.cpp)

TARGET_COMPILE_DEFINITIONS (
    ${PROJECT_NAME}
    PRIVATE -Dnk_printf=printf
            -DUNIT_TEST
            -DNAILONG_KERNEL_SRC_LIBCXX_INCLUDE_NK_NEW_
            -DNAILONG_KERNEL_MAX_CORE_COUNT=4
            -DNAILONG_KERNEL_PER_CPU_ALIGN_SIZE=4096
            -DNAILONG_KERNEL_TICK=1000)

TARGET_INCLUDE_DIRECTORIES (
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/mocks
            ${CMAKE_CURRENT_SOURCE_DIR}/task/fixtures
            ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/src/include
            ${CMAKE_SOURCE_DIR}/src/arch
            ${CMAKE_SOURCE_DIR}/src/libc/include
            ${CMAKE_SOURCE_DIR}/src/libcxx/include
            ${CMAKE_SOURCE_DIR}/src/task/include
            ${CMAKE_SOURCE_DIR}/src/driver/include)

TARGET_COMPILE_OPTIONS (${PROJECT_NAME} PRIVATE ${DEFAULT_TEST_COMPILE_OPTIONS})

TARGET_LINK_OPTIONS (${PROJECT_NAME} PRIVATE ${DEFAULT_TEST_LINK_OPTIONS})

TARGET_LINK_LIBRARIES (${PROJECT_NAME} PRIVATE ${DEFAULT_TEST_LINK_LIB} dtc-lib
                                               bmalloc MPMCQueue task)

GTEST_DISCOVER_TESTS (${PROJECT_NAME})
