cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(
  grid
  VERSION 1.0)

################################################
# grid
add_library(grid grid.cpp)
target_link_libraries(grid PUBLIC exceptions)
target_link_directories(grid PUBLIC ${WASABI_ROOT_DIR}/src/build/lib/)

################################################
# test
add_executable(grid_test grid_test.cpp)
target_link_libraries(grid_test grid)
target_include_directories(grid_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

## [main]
add_test(NAME grid_test COMMAND grid_test)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
