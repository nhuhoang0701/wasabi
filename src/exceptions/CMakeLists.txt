# CMake simple example


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
  DESTINATION $ENV{WASABI_ROOT_DIR}/src/build/lib)
