cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(InA_Interpreter VERSION 1.0 LANGUAGES CXX)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${WASABI_ROOT_DIR}/src/json/include)
include_directories(${WASABI_ROOT_DIR}/src/build/lib)
include_directories(${WASABI_ROOT_DIR}/src/test_tools/include)

link_directories(${WASABI_ROOT_DIR}/external/cJSON/build)
link_directories(${WASABI_ROOT_DIR}/src/build/lib)


add_link_options(LINKER:--export=json_getServerInfo,--export=json_getResponse_json)
add_executable(InA_Interpreter main.cpp)
target_link_libraries(InA_Interpreter cjson jsonUtils.wasm exceptions.wasm)
enable_testing()
add_test(NAME InA_Interpreter_test COMMAND InA_Interpreter)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)

