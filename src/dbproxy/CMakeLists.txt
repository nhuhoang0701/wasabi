cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(
  dbproxy
  VERSION 1.0)
  
################################################
# SQLITE
set(SQLITE_DIR ${WASABI_ROOT_DIR}/external/sqlite/src)
add_definitions(-DNDEBUG -DSQLITE_OS_OTHER)
add_library(sqlite ${SQLITE_DIR}/sqlite3.c ${SQLITE_DIR}/os.c ${SQLITE_DIR}/vfs.c )
target_include_directories(sqlite PUBLIC ${SQLITE_DIR})

################################################
# dbproxy
add_library(dbproxy dbproxy.cpp sqlite/sqlite.cpp)
target_link_libraries(dbproxy PUBLIC sqlite exceptions.wasm)
target_link_directories(dbproxy PUBLIC ${WASABI_ROOT_DIR}/src/build/lib/)

################################################
# test
add_executable(dbproxy_test dbproxy_test.cpp)
target_link_libraries(dbproxy_test dbproxy)
target_include_directories(dbproxy_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

## [main]
enable_testing()
add_test(NAME dbproxy_test COMMAND dbproxy_test --dir=../sqlite/)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
