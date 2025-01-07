cmake_minimum_required(VERSION 3.31)
project(gengine)

option(LUAU_BUILD_CLI "Build CLI" OFF)
option(LUAU_BUILD_TESTS "Build tests" OFF)
option(LUAU_EXTERN_C "Use extern C for all APIs" ON)
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

# Making possible to use Luau API in C
# https://github.com/luau-lang/luau/pull/933#issuecomment-2152968746
foreach(target Luau.VM Luau.Compiler)
  get_target_property(defs ${target} INTERFACE_COMPILE_DEFINITIONS)
  list(FILTER defs EXCLUDE REGEX [[LUA.*_API]])
  set_target_properties(${target} PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${defs}")
endforeach()

add_library(vengine STATIC src/main.c src/veruntime.c src/vestdlib.c src/veinstance.c)
set_property(TARGET vengine PROPERTY C_STANDARD 23)

target_link_libraries(vengine Luau.Compiler Luau.VM raylib)
target_include_directories(vengine PUBLIC include)
