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
  sqlite_example
  VERSION 1.0)

add_definitions(
    -DNDEBUG
    -DSQLITE_OS_OTHER
)

include_directories($ENV{SQLITE_DIR}/src)


add_library(sqlite.wasm $ENV{SQLITE_DIR}/src/sqlite3.c $ENV{SQLITE_DIR}/src/os.c $ENV{SQLITE_DIR}/src/vfs.c )

add_executable(sqlite_example.wasm sqlite_example.cpp )

target_link_libraries(sqlite_example.wasm PRIVATE sqlite.wasm)

enable_testing()
add_test(NAME sqlite_example COMMAND $ENV{WASMTIME} sqlite_example.wasm)

