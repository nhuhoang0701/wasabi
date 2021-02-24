cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)


project(
  sqlite_example
  VERSION 1.0)

set(SQLITE_DIR ${WASABI_ROOT_DIR}/external/sqlite/src)

add_definitions(
    -DNDEBUG
    -DSQLITE_OS_OTHER
)

include_directories(${SQLITE_DIR})


add_library(sqlite ${SQLITE_DIR}/sqlite3.c ${SQLITE_DIR}/os.c ${SQLITE_DIR}/vfs.c )

add_executable(sqlite_example sqlite_example.cpp )

target_link_libraries(sqlite_example PRIVATE sqlite)

add_test(NAME sqlite_example COMMAND sqlite_example)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)

