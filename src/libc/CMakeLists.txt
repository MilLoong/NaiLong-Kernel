# Copyright The NaiLong-Kernel Contributors

ADD_LIBRARY (libc INTERFACE)

TARGET_INCLUDE_DIRECTORIES (libc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

TARGET_SOURCES (
    libc
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/nk_stdlib.c
              ${CMAKE_CURRENT_SOURCE_DIR}/nk_string.c
              ${CMAKE_CURRENT_SOURCE_DIR}/nk_stdio.c
              ${CMAKE_CURRENT_SOURCE_DIR}/nk_ctype.c)
