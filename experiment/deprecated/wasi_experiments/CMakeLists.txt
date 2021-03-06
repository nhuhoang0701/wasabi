# CMake simple example

set(WASABI_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set(WASABI_EXTERNAL ${WASABI_ROOT}/external)

## [main]
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR wasm)
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CLANG_TARGET_TRIPLE wasm32-unknown-wasi)
set(CMAKE_SYSROOT ${WASABI_EXTERNAL}/wasi-sdk-12/wasi-sysroot)

set(CMAKE_C_COMPILER ${LLVM_DIR}/bin/clang)
set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_CXX_COMPILER ${LLVM_DIR}/bin/clang++)
set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_AR ${LLVM_DIR}/bin/llvm-ar)
set(CMAKE_SPLIT ${LLVM_DIR}/bin/llvm-split)
set(CMAKE_BUILD_TYPE Debug)    
#set( CMAKE_VERBOSE_MAKEFILE on ) 

# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.1...3.19)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  ModernCMakeExample
  VERSION 1.0
  LANGUAGES CXX)

# If you set any CMAKE_ variables, that can go here.
# (But usually don't do this, except maybe for C++ standard)

# Find packages go here.

# You should usually split this into folders, but this is a simple example

# This is a "default" library, and will match the *** variable setting.
# Other common choices are STATIC, SHARED, and MODULE
# Including header files here helps IDEs but is not required.
# Output libname matches target name, with the usual extensions on your system
add_library(MyLibExample simple_lib.cpp simple_lib.hpp)

# Link each target with other targets or add options, etc.

# Adding something we can run - Output name matches target name
add_executable(MyExample simple_example.cpp)

# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error.
target_link_libraries(MyExample PRIVATE MyLibExample)

## [main]

# This part is so the Modern CMake book can verify this example builds. For your code,
# you'll probably want tests too
enable_testing()
add_test(NAME MyExample COMMAND MyExample)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)

