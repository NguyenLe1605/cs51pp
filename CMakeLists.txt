cmake_minimum_required(VERSION 3.26)
project(cs51pp LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-g)

add_subdirectory("labs")
add_subdirectory("psets")
add_subdirectory("third_party")

add_subdirectory("tests")
find_program(CS51CXX clang++ HINTS /lib/llvm-18/bin /usr/bin REQUIRED)
set(CMAKE_CXX_COMPILER ${CS51CXX})
