cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(
  cube
  VERSION 1.0)
  
################################################
# SQLITE
set(SQLITE_DIR ${WASABI_ROOT_DIR}/external/sqlite/src)
add_definitions(-DNDEBUG -DSQLITE_OS_OTHER)
add_library(sqlite ${SQLITE_DIR}/sqlite3.c ${SQLITE_DIR}/os.c ${SQLITE_DIR}/vfs.c )
target_include_directories(sqlite PUBLIC ${SQLITE_DIR})

################################################
# cube
add_library(cube cube.cpp)
target_link_libraries(cube PUBLIC exceptions.wasm)
target_link_directories(cube PUBLIC ${WASABI_ROOT_DIR}/src/build/lib/)

################################################
# test
add_executable(cube_test cube_test.cpp)
target_link_libraries(cube_test cube)
target_include_directories(cube_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

## [main]
enable_testing()
add_test(NAME cube_test COMMAND cube_test)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
