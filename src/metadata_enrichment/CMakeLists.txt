cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(metadata_enrichment VERSION 1.0 LANGUAGES CXX)

################################################
# metadata_enrichment
add_library(metadata_enrichment Tables.cpp)
target_link_libraries(metadata_enrichment PUBLIC json cjson sqlite dbproxy exceptions)
target_link_directories(metadata_enrichment PUBLIC ${cJSON_LIBRARY_DIRS}  ${WASABI_LIBS})

################################################
# test
add_executable(metadata_enrichment_test metadata_enrichment_test.cpp Tables_test.cpp)
target_include_directories(metadata_enrichment_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)
target_link_libraries(metadata_enrichment_test PRIVATE metadata_enrichment)

# run with ctest -V
add_test(NAME metadata_enrichment_test COMMAND metadata_enrichment_test --dir=./  WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})

## [installation]
install(TARGETS metadata_enrichment DESTINATION lib)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
