
set(WASABI_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set(WASABI_EXTERNAL ${WASABI_ROOT}/external)

## [main]
set(CMAKE_SYSTEM_NAME WAsm)

set(CLANG_TARGET_TRIPLE wasm32-unknown-wasi)
set(CMAKE_SYSROOT ${SYSROOT_DIR})

set(CMAKE_C_COMPILER ${C_COMPILER})
set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_CXX_COMPILER ${CXX_COMPILER})
set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})


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

#add_library(MyLibExample simple_lib.cpp simple_lib.hpp)
#add_library(C_libc_dl C_libc_dl.c)

# Link each target with other targets or add options, etc.

# Adding something we can run - Output name matches target name
add_executable(C_libc.wasm C_libc.c)
add_executable(C_nolib.wasm C_nolib.c)
#add_executable(C_libc_dl.wasm C_libc_dl.c)



# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error.
#target_link_libraries(C_libc.wasm PRIVATE )
#target_link_libraries(C_libc_dl.wasm PRIVATE )


## [main]

# This part is so the Modern CMake book can verify this example builds. For your code,
# you'll probably want tests too
enable_testing()
add_test(NAME C_nolib COMMAND ${WASMTIME} C_nolib.wasm)
## check --dir if usable or not, it's for open file
add_test(NAME C_libc COMMAND ${WASMTIME} C_libc.wasm)


