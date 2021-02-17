cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(InA_Interpreter VERSION 1.0 LANGUAGES CXX)

add_link_options(LINKER:--export=json_getServerInfo,--export=json_getResponse_json)
add_executable(InA_Interpreter main.cpp)

enable_testing()
add_test(NAME InA_Interpreter_test COMMAND InA_Interpreter)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)

