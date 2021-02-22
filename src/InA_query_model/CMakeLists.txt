cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(
  InA_query_model
  VERSION 1.0)
  
################################################
# SQLITE
set(SQLITE_DIR ${WASABI_ROOT_DIR}/external/sqlite/src)
add_definitions(-DNDEBUG -DSQLITE_OS_OTHER)
add_library(sqlite ${SQLITE_DIR}/sqlite3.c ${SQLITE_DIR}/os.c ${SQLITE_DIR}/vfs.c )
target_include_directories(sqlite PUBLIC ${SQLITE_DIR})

################################################
# InA_query_model
add_library(InA_query_model InA_query_model.cpp)
target_link_libraries(InA_query_model PUBLIC exceptions.wasm)
target_link_directories(InA_query_model PUBLIC ${WASABI_ROOT_DIR}/src/build/lib/)

################################################
# test
add_executable(InA_query_model_test InA_query_model_test.cpp)
target_link_libraries(InA_query_model_test InA_query_model)
target_include_directories(InA_query_model_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

## [main]
enable_testing()
add_test(NAME InA_query_model_test COMMAND InA_query_model_test)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
