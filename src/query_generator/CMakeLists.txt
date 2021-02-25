cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(
  query_generator
  VERSION 1.0)


################################################
# query_generator
set(QUERY_MODEL_LIB ${WASABI_ROOT_DIR}/src/InA_query_model)
set(QUERY_MODEL_LIB_BUILD ${QUERY_MODEL_LIB}/build)
set(GRID_LIB ${WASABI_ROOT_DIR}/src/grid)
add_library(query_generator query_generator.cpp)
target_include_directories(query_generator PUBLIC ${QUERY_MODEL_LIB} ${GRID_LIB})
target_link_libraries(query_generator PUBLIC exceptions InA_query_model)
target_link_directories(query_generator PUBLIC ${WASABI_ROOT_DIR}/src/build/lib/ ${QUERY_MODEL_LIB_BUILD})

################################################
# test
add_executable(query_generator_test query_generator_test.cpp)
target_include_directories(query_generator_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include ${QUERY_MODEL_LIB} ${GRID_LIB})
target_link_libraries(query_generator_test PUBLIC query_generator InA_query_model)
target_link_directories(query_generator_test PUBLIC ${QUERY_MODEL_LIB_BUILD})

## [main]
add_test(NAME query_generator_test COMMAND query_generator_test)

## [installation]
install(TARGETS query_generator DESTINATION ${WASABI_ROOT_DIR}/src/build/lib)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
