# Copyright The NaiLong-Kernel Contributors

ADD_LIBRARY (pl011 INTERFACE)

# console_driver.h 位于 ${CMAKE_CURRENT_SOURCE_DIR}/../include，一并暴露，
# 使本驱动不经 driver 聚合库被独立引用时也能 #include <console_driver.h>。
TARGET_INCLUDE_DIRECTORIES (pl011
                            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
                                      ${CMAKE_CURRENT_SOURCE_DIR}/../include)

TARGET_SOURCES (pl011 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/pl011.cpp)
