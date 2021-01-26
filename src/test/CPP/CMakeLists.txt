# CMake simple example for CPP

set(WASABI_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set(WASABI_EXTERNAL ${WASABI_ROOT}/external)
## [main]
set(CMAKE_SYSTEM_NAME WAsm)

set(CLANG_TARGET_TRIPLE wasm32-unknown-wasi)
set(CMAKE_SYSROOT ${WASABI_EXTERNAL}/wasi-sdk-12/wasi-sysroot)

set(CMAKE_C_COMPILER ${WASABI_EXTERNAL}/llvm/bin/clang)
set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_CXX_COMPILER ${WASABI_EXTERNAL}/llvm/bin/clang++)
set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_CXX_FLAGS "-fdeclspec ")

set( CMAKE_VERBOSE_MAKEFILE on ) 

cmake_minimum_required(VERSION 3.1...3.19)

project(
  Wasabi_ValidationCPP_Support
  VERSION 1.0
  LANGUAGES CXX)

link_directories(${WASABI_ROOT}/src/build/lib)

add_library(CPP_libcxx.wasm CPP_libcxx.cpp )

add_executable(CPP_nolib_class.wasm ../CPP_nolib_class.cpp)
add_executable(CPP_nolib_exception.wasm ../CPP_nolib_exception.cpp)
add_executable(CPP_Withlibcxx.wasm ../CPP_Withlibcxx.cpp )


target_link_libraries(CPP_nolib_exception.wasm PRIVATE exceptions.wasm )
target_link_libraries(CPP_Withlibcxx.wasm PRIVATE CPP_libcxx.wasm )

## [main]

enable_testing()
add_test(NAME CPP_nolib_class COMMAND wasmtime CPP_nolib_class.wasm)
add_test(NAME CPP_nolib_exception COMMAND wasmtime CPP_nolib_exception.wasm)
add_test(NAME CPP_Withlibcxx COMMAND wasmtime CPP_Withlibcxx.wasm)
