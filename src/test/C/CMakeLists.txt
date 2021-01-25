# CMake simple example for C

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

set(CMAKE_C_FLAGS "-fdeclspec")
set(CMAKE_VERBOSE_MAKEFILE on ) 


# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.1...3.19)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  Wasabi_ValidationC_Support
  VERSION 1.0
  LANGUAGES C)

# If you set any CMAKE_ variables, that can go here.
# (But usually don't do this, except maybe for C++ standard)

# Find packages go here.

# You should usually split this into folders, but this is a simple example

# This is a "default" library, and will match the *** variable setting.
# Other common choices are STATIC, SHARED, and MODULE
# Including header files here helps IDEs but is not required.
# Output libname matches target name, with the usual extensions on your system
#add_library(MyLibExample simple_lib.cpp simple_lib.hpp)
#add_library(C_libc_dl C_libc_dl.c)

# Link each target with other targets or add options, etc.

# Adding something we can run - Output name matches target name
add_executable(C_libc C_libc.c)
#add_executable(C_nolib C_nolib.c)
#add_executable(C_libc_dl C_libc_dl.c)



# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error.
#target_link_libraries(C_libc PRIVATE )
#target_link_libraries(C_nolib PRIVATE )
#target_link_libraries(C_libc_dl PRIVATE )


## [main]

# This part is so the Modern CMake book can verify this example builds. For your code,
# you'll probably want tests too
enable_testing()
add_test(NAME C_libc COMMAND wasmtime --dir=. C_libc)
#add_test(NAME C_nolib COMMAND wasmtime C_nolib)

