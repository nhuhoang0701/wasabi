cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(InA_Interpreter VERSION 1.0 LANGUAGES CXX)

set(QUERY_MODEL_LIB ${WASABI_ROOT_DIR}/src/InA_query_model)
set(QUERY_MODEL_LIB_BUILD ${QUERY_MODEL_LIB}/build)
set(QUERY_GENERATOR_LIB ${WASABI_ROOT_DIR}/src/query_generator)
set(QUERY_GENERATOR_LIB_BUILD ${QUERY_GENERATOR_LIB}/build)
set(CUBE_LIB ${WASABI_ROOT_DIR}/src/cube)
#set(CUBE_LIB_BUILD ${CUBE_LIB}/build)
set(DB_PROXY_LIB ${WASABI_ROOT_DIR}/src/dbproxy)
#set(DB_PROXY_LIB_BUILD ${DB_PROXY_LIB}/build)
set(GRID_LIB ${WASABI_ROOT_DIR}/src/grid)
#set(GRID_LIB_BUILD ${GRID_LIB}/build)

################################################
# InA_Interpreter

# InA_Interpreter as library
add_library(InA_Interpreter_lib InA_Interpreter.cpp)
target_include_directories(InA_Interpreter_lib PUBLIC  ${PROJECT_SOURCE_DIR}/include ${WASABI_ROOT_DIR}/src/json/include ${QUERY_MODEL_LIB} ${QUERY_GENERATOR_LIB} ${GRID_LIB} ${CUBE_LIB})
target_link_libraries(InA_Interpreter_lib PUBLIC cjson jsonUtils exceptions InA_query_model query_generator)
target_link_directories(InA_Interpreter_lib PUBLIC ${WASABI_ROOT_DIR}/external/cJSON/build ${WASABI_ROOT_DIR}/src/build/lib ${QUERY_MODEL_LIB_BUILD} ${QUERY_GENERATOR_LIB_BUILD})


# InA_Interpreter as executable to be sure of C/C++ init staff is well done
add_executable(InA_Interpreter InA_Interpreter_main.cpp)
# External entries points
target_link_options( InA_Interpreter PRIVATE LINKER:--export=json_getServerInfo,--export=json_getResponse_json,--export=malloc,--export=free)
# To allow to link with InA_Interpreter executable
set_property(TARGET InA_Interpreter PROPERTY ENABLE_EXPORTS 1)
target_include_directories(InA_Interpreter PUBLIC ${PROJECT_SOURCE_DIR}/include ${WASABI_ROOT_DIR}/src/json/include)

target_link_directories(InA_Interpreter PUBLIC ${WASABI_ROOT_DIR}/external/cJSON/build ${WASABI_ROOT_DIR}/src/build/lib)
target_link_libraries(InA_Interpreter PUBLIC InA_Interpreter_lib cjson jsonUtils exceptions)


################################################
# test
add_executable(InA_Interpreter_test InA_Interpreter_test.cpp)
target_link_libraries(InA_Interpreter_test InA_Interpreter_lib)
target_include_directories(InA_Interpreter_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

add_test(NAME InA_Interpreter_test COMMAND InA_Interpreter_test)

## [installation]
install(TARGETS InA_Interpreter DESTINATION ${WASABI_ROOT_DIR}/src/wasi_browser)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)

