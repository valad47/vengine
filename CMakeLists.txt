cmake_minimum_required(VERSION 3.31)
project(gengine)

add_subdirectory(luau)

add_library(vengine src/main.c)

target_link_libraries(vengine Luau.Compiler Luau.VM)
target_include_directories(vengine PRIVATE include)