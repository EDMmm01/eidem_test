cmake_minimum_required(VERSION 3.15)

# 同时声明支持 C 和 CXX
project(demo_mix LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)

# CMake 会自动用 C 编译器编 add.c，用 C++ 编译器编 main.cpp，并使用 g++ 进行最终链接
add_executable(app_mix main.cpp add.c)
