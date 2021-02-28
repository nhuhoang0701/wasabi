cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(cube VERSION 1.0)
  
set(DB_PROXY_LIB ${WASABI_ROOT_DIR}/src/dbproxy)
#set(DB_PROXY_LIB_BUILD ${DB_PROXY_LIB}/build)

################################################
# cube
add_library(cube cube.cpp)
target_include_directories(cube PUBLIC ${DB_PROXY_LIB})
target_link_libraries(cube PUBLIC exceptions)
target_link_directories(cube PUBLIC ${CMAKE_INSTALL_PREFIX}/lib/)

################################################
# test
add_executable(cube_test cube_test.cpp)
target_include_directories(cube_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include ${DB_PROXY_LIB})
target_link_libraries(cube_test cube)
target_link_directories(cube_test PUBLIC)

## [main]
add_test(NAME cube_test COMMAND cube_test)

## [installation]
install(TARGETS cube DESTINATION lib)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
