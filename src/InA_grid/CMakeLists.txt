cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(InA_grid VERSION 1.0)

################################################
# InA_grid
add_library(InA_grid grid.cpp gridWriter.cpp)
set_target_properties(InA_grid PROPERTIES PUBLIC_HEADER grid.h)

target_link_libraries(InA_grid PUBLIC exceptions)
target_link_directories(InA_grid PUBLIC ${WASABI_LIBS})

################################################
# test
add_executable(InA_grid_test grid_test.cpp)
target_link_libraries(InA_grid_test InA_grid)
target_include_directories(InA_grid_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

## [main]
add_test(NAME InA_grid COMMAND InA_grid_test)

## [installation]
install(TARGETS InA_grid 
		LIBRARY DESTINATION ${WASABI_LIBS}
		PUBLIC_HEADER DESTINATION ${WASABI_INCLUDES})

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
