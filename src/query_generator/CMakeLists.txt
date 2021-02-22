cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(
  query_generator
  VERSION 1.0)
  
################################################
# SQLITE
set(SQLITE_DIR ${WASABI_ROOT_DIR}/external/sqlite/src)
add_definitions(-DNDEBUG -DSQLITE_OS_OTHER)
add_library(sqlite ${SQLITE_DIR}/sqlite3.c ${SQLITE_DIR}/os.c ${SQLITE_DIR}/vfs.c )
target_include_directories(sqlite PUBLIC ${SQLITE_DIR})

################################################
# query_generator
add_library(query_generator query_generator.cpp)
target_link_libraries(query_generator PUBLIC exceptions.wasm)
target_link_directories(query_generator PUBLIC ${WASABI_ROOT_DIR}/src/build/lib/)

################################################
# test
add_executable(query_generator_test query_generator_test.cpp)
target_link_libraries(query_generator_test query_generator)
target_include_directories(query_generator_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

## [main]
enable_testing()
add_test(NAME query_generator_test COMMAND query_generator_test)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
