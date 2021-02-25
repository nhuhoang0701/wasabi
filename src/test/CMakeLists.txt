cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(wasabi_src_test VERSION 1.0)


add_subdirectory(C)
add_subdirectory(CPP)
add_subdirectory(sqlite_example)
add_subdirectory(wasi)
#add_subdirectory(basic_wasm_jsapi)


################################################
# 
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
