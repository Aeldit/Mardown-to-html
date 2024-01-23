cmake_minimum_required(VERSION 3.20)

project(MarkdownToHtml)

# Compiler flags
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_C_STANDARD 11)

add_library(handling
    STATIC
        files_handling.h
        files_handling.c
)

add_compile_options(
        -Wall
        -pedantic
        -std=c99
)

add_executable(MarkdownToHtml main.c)

target_link_libraries(MarkdownToHtml PRIVATE handling)