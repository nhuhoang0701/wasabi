cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(
  metadata_enrichment
  VERSION 1.0)

################################################
# metadata_enrichment
add_library(metadata_enrichment metadata_enrichment.cpp)
target_link_libraries(metadata_enrichment PUBLIC exceptions.wasm)
target_link_directories(metadata_enrichment PUBLIC ${WASABI_ROOT_DIR}/src/build/lib/)

################################################
# test
add_executable(metadata_enrichment_test metadata_enrichment_test.cpp)
target_link_libraries(metadata_enrichment_test metadata_enrichment)
target_include_directories(metadata_enrichment_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

## [main]
add_test(NAME metadata_enrichment_test COMMAND metadata_enrichment_test)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
