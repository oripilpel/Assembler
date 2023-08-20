cmake_minimum_required(VERSION 3.0)

project(assembler)

set(CMAKE_C_STANDARD 90)

add_executable(assembler assembler.c file.c macro.c label.c)

target_compile_options(assembler PRIVATE -g -ansi -Wall -pedantic)
