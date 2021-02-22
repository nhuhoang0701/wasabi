cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(InA_Interpreter VERSION 1.0 LANGUAGES CXX)


################################################
# InA_Interpreter

# External entries points
add_link_options(LINKER:--export=json_getServerInfo,--export=json_getResponse_json)

add_library(InA_Interpreter main.cpp)
target_include_directories(InA_Interpreter PUBLIC ${WASABI_ROOT_DIR}/src/json/include)
target_include_directories(InA_Interpreter PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_directories(InA_Interpreter PUBLIC ${WASABI_ROOT_DIR}/external/cJSON/build ${WASABI_ROOT_DIR}/src/build/lib)
target_link_libraries(InA_Interpreter PUBLIC cjson jsonUtils.wasm exceptions.wasm)

################################################
# test
add_executable(InA_Interpreter_test main.cpp)
target_link_libraries(InA_Interpreter_test InA_Interpreter)
target_include_directories(InA_Interpreter_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

enable_testing()
add_test(NAME InA_Interpreter_test COMMAND InA_Interpreter_test)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)

