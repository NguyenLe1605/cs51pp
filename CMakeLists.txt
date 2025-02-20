cmake_minimum_required(VERSION 3.28)
project(cs51pp LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_subdirectory("labs")
add_subdirectory("psets")
add_subdirectory("third_party")

add_subdirectory("tests")

find_program(CS51CXX clang++ HINTS /usr/bin REQUIRED)
set(CMAKE_CXX_COMPILER ${CS51CXX})
set(CMAKE_CXX_STANDARD 23)
