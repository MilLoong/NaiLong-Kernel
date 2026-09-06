# Copyright The NaiLong-Kernel Contributors

ADD_LIBRARY (libcxx INTERFACE)

TARGET_INCLUDE_DIRECTORIES (libcxx
                            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

TARGET_SOURCES (
    libcxx
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/nk_libcxx.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/nk_new.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/nk_iostream.cpp)
