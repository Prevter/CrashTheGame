cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if (WIN32)
    set(CMAKE_ASM_NASM_COMPILER nasm)
    set(CMAKE_ASM_NASM_FLAGS "")
    set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -f win64 -o <OBJECT> <SOURCE>")
    project(CrashTheGame VERSION 1.0.0 LANGUAGES CXX ASM_NASM)
else()
    project(CrashTheGame VERSION 1.0.0)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Enable C++ exceptions for Clang-cl
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Xclang -fcxx-exceptions)
endif()

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/utils.cpp src/utils.hpp
    src/CrashEngine.cpp src/CrashEngine.hpp
    src/CrashPickerLayer.cpp src/CrashPickerLayer.hpp
)

# on 64 bit windows, include "Win64Utils.asm"
if (WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_sources(${PROJECT_NAME} PRIVATE src/Win64Utils.asm)
    set_source_files_properties(src/Win64Utils.asm PROPERTIES LANGUAGE ASM_NASM)
endif()

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
