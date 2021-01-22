# CMake simple example for CPP

set(WASABI_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

## [main]
set(CMAKE_SYSTEM_NAME WAsm)

set(CLANG_TARGET_TRIPLE wasm32-unknown-wasi)
set(CMAKE_SYSROOT ${WASABI_ROOT}/wasi-sdk-12/wasi-sysroot)

set(CMAKE_C_COMPILER ${WASABI_ROOT}/llvm/bin/clang)
set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_CXX_COMPILER ${WASABI_ROOT}/llvm/bin/clang++)
set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

#SET(CMAKE_AR ${WASABI_ROOT}/llvm/bin/llvm-ar)
#SET(CMAKE_SPLIT ${WASABI_ROOT}/llvm/bin/llvm-split)

set(CMAKE_CXX_FLAGS "-fdeclspec ")
# -fno-exceptions for no exception
#-fwasm-exceptions

set( CMAKE_VERBOSE_MAKEFILE on ) 

# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.1...3.19)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  Wasabi_ValidationCPP_Support
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
add_library(CPP_libcxx ../CPP_libcxx.cpp )

# Link each target with other targets or add options, etc.

# Adding something we can run - Output name matches target name
add_executable(CPP_nolib_class ../CPP_nolib_class.cpp)
add_executable(CPP_nolib_exception ../CPP_nolib_exception.cpp)
add_executable(CPP_Withlibcxx ../CPP_Withlibcxx.cpp )

# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error.
#target_link_libraries(CPP_libcxx PRIVATE )
#target_link_libraries(CPP_nolib_class PRIVATE )
#target_link_libraries(CPP_nolib_exception PRIVATE ${CMAKE_SYSROOT}/lib/wasm32-wasi/libc++abi )
target_link_libraries(CPP_Withlibcxx PRIVATE CPP_libcxx )

## [main]

# This part is so the Modern CMake book can verify this example builds. For your code,
# you'll probably want tests too
enable_testing()
add_test(NAME CPP_nolib_class COMMAND wasmtime CPP_nolib_class)
add_test(NAME CPP_nolib_exception COMMAND wasmtime CPP_nolib_exception)
add_test(NAME CPP_Withlibcxx COMMAND wasmtime CPP_Withlibcxx)

