cmake_minimum_required(VERSION 3.31)
project(gengine)

#option(LUAU_EXTERN_C "Use extern C for all APIs" ON)
add_subdirectory(luau)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(RAYLIB_VERSION 4.5.0)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)

FetchContent_MakeAvailable(raylib)

add_library(vengine src/main.c src/veruntime.c src/vestdlib.c)
set_property(TARGET vengine PROPERTY C_STANDARD 23)

target_link_libraries(vengine Luau.Compiler Luau.VM raylib)
target_include_directories(vengine PRIVATE include)
