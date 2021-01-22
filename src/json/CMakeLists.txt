# CMake json

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

# WASI does not support exceptions
#add_compile_options(-fno-exceptions) # for this project and all sub-projects

# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.1...3.19)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  JsonUtils
  VERSION 1.0
  LANGUAGES CXX)





add_subdirectory(src)

add_subdirectory(test)
