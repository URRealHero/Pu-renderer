cmake_minimum_required(VERSION 3.1.0)

project(SimpleRT LANGUAGES CXX)

# 设置C++标准版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加头文件路径
include_directories(src)

# 源文件
set(SOURCES
    src/main.cpp
    src/color.cpp

    src/color.h
    src/vec3.h
)

# 创建可执行文件
add_executable(main ${SOURCES})

# 可选：根据编译器添加警告选项（推荐）
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(main PRIVATE -Wall -Wextra -pedantic)
endif()