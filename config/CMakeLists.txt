cmake_minimum_required(VERSION 3.16)


# Set variables onnly
add_subdirectory(cmake)
add_subdirectory(ninja)
add_subdirectory(clang)

# Download or build
add_subdirectory(binaryen)
add_subdirectory(wasmtime)
add_subdirectory(wasmsysroot)
add_subdirectory(sqlite)
add_subdirectory(cjson)
