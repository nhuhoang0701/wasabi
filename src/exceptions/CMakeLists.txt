# CMake simple example

set(WASABI_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(WASABI_EXTERNAL ${WASABI_ROOT}/external)

## [main]
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR wasm)
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CLANG_TARGET_TRIPLE wasm32-unknown-wasi)
set(CMAKE_SYSROOT ${WASABI_EXTERNAL}/wasi-sdk-12/wasi-sysroot)

set(CMAKE_C_COMPILER ${WASABI_EXTERNAL}/llvm/bin/clang)
set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_CXX_COMPILER ${WASABI_EXTERNAL}/llvm/bin/clang++)
set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_AR ${WASABI_EXTERNAL}/llvm/bin/llvm-ar)
set(CMAKE_SPLIT ${WASABI_EXTERNAL}/llvm/bin/llvm-split)

# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.1...3.19)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  Exceptions
  VERSION 1.0
  LANGUAGES CXX)

# If you set any CMAKE_ variables, that can go here.
# (But usually don't do this, except maybe for C++ standard)

# Find packages go here.

# You should usually split this into folders, but this is a simple example

# Link each target with other targets or add options, etc.

#include_directories(${WASABI_EXTERNAL})
#include_directories(${PROJECT_SOURCE_DIR}/include)





add_library(exceptions.wasm cxa_exception.cpp)



install(TARGETS exceptions.wasm
  DESTINATION ${WASABI_ROOT}/src/build/lib)
