# This file is a part of MRNIU/cpu_io (https://github.com/MRNIU/cpu_io).
#
# CMakeLists.txt for MRNIU/cpu_io.

# 设置最小 cmake 版本
CMAKE_MINIMUM_REQUIRED (VERSION 3.27 FATAL_ERROR)

# 设置项目名与版本
PROJECT (cpu_io VERSION 0.0.1)

# 禁止原地编译
IF(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    # 如果你看到这句话，cmake 此时已经在根目录下生成了一些临时文件，你需要删除它们 CMakeFiles, CMakeCache.txt
    MESSAGE (
        FATAL_ERROR
            "In-source builds not allowed."
            "Please make a new directory (called a build directory) "
            "and run CMake from there.")
ENDIF()

ADD_LIBRARY (${PROJECT_NAME} INTERFACE)
TARGET_INCLUDE_DIRECTORIES (${PROJECT_NAME} INTERFACE include)

# CONFIGURE_FILE (${PROJECT_SOURCE_DIR}/.pre-commit-config.yaml.in
# ${PROJECT_SOURCE_DIR}/.pre-commit-config.yaml @ONLY)

# 获取 gcc 的 include 路径
EXECUTE_PROCESS (
    COMMAND sh -c "echo | ${CMAKE_CXX_COMPILER} -v -x c -E - 2>&1 | sed -n \
        '/#include <...> search starts here:/,/End of search list./{/^ /p}'"
    OUTPUT_VARIABLE GCC_OUTPUT
    ERROR_VARIABLE GCC_ERROR
    RESULT_VARIABLE GCC_RESULT
    OUTPUT_STRIP_TRAILING_WHITESPACE)
# 检查 gcc 是否成功执行
IF(NOT GCC_RESULT EQUAL 0)
    MESSAGE (FATAL_ERROR "Failed to run ${CMAKE_CXX_COMPILER} -v")
ENDIF()
# 分割路径并生成路径列表
STRING (REPLACE "\n" ";" INCLUDE_PATH_LIST "${GCC_OUTPUT}")
# 使用 `-I` 将路径添加到编译选项中
FOREACH(item ${INCLUDE_PATH_LIST})
    STRING (REGEX REPLACE " " "" CLEAN_PATH ${item})
    LIST (APPEND CROSS_INCLUDE_PATHS "-I${CLEAN_PATH}")
ENDFOREACH()
MESSAGE (STATUS "GCC Include CROSS_INCLUDE_PATHS: ${CROSS_INCLUDE_PATHS}")

ADD_EXECUTABLE (
    cpu_io_test
    # operator.cpp
    test.cpp)

TARGET_COMPILE_OPTIONS (cpu_io_test PRIVATE ${CROSS_INCLUDE_PATHS})
TARGET_LINK_LIBRARIES (cpu_io_test PRIVATE ${PROJECT_NAME})
