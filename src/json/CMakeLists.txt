# CMake json


## [main]
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR wasm)
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CLANG_TARGET_TRIPLE wasm32-unknown-wasi)
set(CMAKE_SYSROOT $ENV{SYSROOT_DIR})

set(CMAKE_C_COMPILER $ENV{C_COMPILER})
set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_CXX_COMPILER $ENV{CXX_COMPILER})
set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_AR $ENV{LLVM_AR})
set(CMAKE_SPLIT $ENV{LLVM_SPLIT})

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



# generate a "test" target (this command must be issued at project's root)
enable_testing()

add_subdirectory(src)

add_subdirectory(test)
