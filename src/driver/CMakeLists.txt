# Copyright The NaiLong-Kernel Contributors

# driver 是一个纯接口聚合库（INTERFACE），职责是：
# 1. 暴露 src/driver/include/ 下的公共头文件（如 console_driver.h）
# 2. 按架构将对应的驱动子库链接给上层模块
# 注意：不再包含 driver.cpp（原实现为空死循环，已移除）

ADD_LIBRARY(driver INTERFACE)

TARGET_INCLUDE_DIRECTORIES(driver
                            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

TARGET_LINK_LIBRARIES(
    driver
    INTERFACE $<$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},riscv64>:
              ns16550a
              plic>
              $<$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},aarch64>:
              pl011
              gic>
              $<$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>:
              apic
              acpi>)

IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "riscv64")
    ADD_SUBDIRECTORY(ns16550a)
    ADD_SUBDIRECTORY(plic)
ELSEIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    ADD_SUBDIRECTORY(pl011)
    ADD_SUBDIRECTORY(gic)
ELSEIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    ADD_SUBDIRECTORY(apic)
    ADD_SUBDIRECTORY(acpi)
ENDIF()
