# Basic CMake to build

cmake_minimum_required(VERSION 3.15)
project(sef)

set(COLOR_RESET   "\\033[0m")
set(COLOR_RED     "\\033[0;31m")
set(COLOR_GREEN   "\\033[0;32m")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(_Version_Major 0)
set(_Version_Minor 0)
set(_Version_Patch 0)
set(_Version_Status "-dev")
set(PROJECT_VERSION
    "${_Version_Major}.${_Version_Minor}.${_Version_Patch}${_Version_Status}"
)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

set(
    SEF_SOURCES
    src/args.c
    src/context.c
    src/core.c
    src/error.c
    src/registry.c
    src/writer.c
    src/size.c
)

include_directories(src/inc)
add_compile_options(-Wall -Wextra -Wpedantic)

option(DISABLE_SHARED "No shared library" OFF)
option(DISABLE_STATIC "No static library" OFF)

if (NOT DISABLE_SHARED)
    message(STATUS "Shared library included")
    add_library(sef_shared SHARED ${SEF_SOURCES})
    set_target_properties(sef_shared PROPERTIES OUTPUT_NAME "sef")
else()
    message(NOTICE "Shared library excluded")
endif()

if (NOT DISABLE_STATIC)
    message(STATUS "Static library included")
    add_library(sef_static STATIC ${SEF_SOURCES})
    set_target_properties(sef_static PROPERTIES OUTPUT_NAME "sef")
else()
    message(NOTICE "Static library excluded")
endif()

if (DISABLE_SHARED AND DISABLE_STATIC)
    message(FATAL_ERROR ":) Do not exclude both static and shared libraries")
endif()
