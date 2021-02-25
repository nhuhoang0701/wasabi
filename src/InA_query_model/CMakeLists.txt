cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(
  InA_query_model
  VERSION 1.0)

set(GRID_LIB ${WASABI_ROOT_DIR}/src/grid)
#set(GRID_LIB_BUILD ${GRID_LIB}/build)
  
################################################
# InA_query_model
add_library(InA_query_model InA_query_model.cpp)
target_include_directories(InA_query_model PUBLIC ${GRID_LIB})
target_link_libraries(InA_query_model PUBLIC exceptions)
target_link_directories(InA_query_model PUBLIC ${WASABI_ROOT_DIR}/src/build/lib/)

################################################
# test
add_executable(InA_query_model_test InA_query_model_test.cpp)
target_include_directories(InA_query_model_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include ${GRID_LIB})
target_link_libraries(InA_query_model_test InA_query_model)
target_link_directories(InA_query_model_test PUBLIC)

## [main]
add_test(NAME InA_query_model_test COMMAND InA_query_model_test)

## [installation]
install(TARGETS InA_query_model DESTINATION ${WASABI_ROOT_DIR}/src/build/lib)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
