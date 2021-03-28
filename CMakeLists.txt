cmake_minimum_required(VERSION 3.16)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_COMPILER "clang-11")
set(CMAKE_CXX_COMPILER "clang++-11")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(nutspack)

add_subdirectory(service)