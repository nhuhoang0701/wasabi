# CMake simple example for CPP

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

cmake_minimum_required(VERSION 3.1...3.19)

project(
  Wasabi_ValidationCPP_Support
  VERSION 1.0
  LANGUAGES CXX)

include_directories($ENV{WASABI_ROOT_DIR}/src/test_tools/include)
link_directories($ENV{WASABI_ROOT_DIR}/src/build/lib)

add_library(CPP_libcxx.wasm CPP_libcxx.cpp )

add_executable(CPP_nolib_class.wasm ../CPP_nolib_class.cpp)
add_executable(CPP_nolib_exception.wasm ../CPP_nolib_exception.cpp)
add_executable(CPP_Withlibcxx.wasm ../CPP_Withlibcxx.cpp )


target_link_libraries(CPP_nolib_exception.wasm PRIVATE exceptions.wasm )
target_link_libraries(CPP_Withlibcxx.wasm PRIVATE CPP_libcxx.wasm exceptions.wasm )

## [main]

enable_testing()
add_test(NAME CPP_nolib_class COMMAND $ENV{WASMTIME} CPP_nolib_class.wasm)
add_test(NAME CPP_nolib_exception COMMAND $ENV{WASMTIME} CPP_nolib_exception.wasm)
add_test(NAME CPP_Withlibcxx COMMAND $ENV{WASMTIME} CPP_Withlibcxx.wasm)
