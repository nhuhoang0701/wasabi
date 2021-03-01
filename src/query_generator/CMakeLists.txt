cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(query_generator VERSION 1.0)


################################################
# query_generator
add_library(query_generator query_generator.cpp)
set_target_properties(query_generator PROPERTIES PUBLIC_HEADER query_generator.h)

target_include_directories(query_generator PUBLIC ${WASABI_INCLUDES})
target_link_libraries(query_generator PUBLIC exceptions InA_query_model)
target_link_directories(query_generator PUBLIC ${WASABI_LIBS})

################################################
# test
add_executable(query_generator_test query_generator_test.cpp)
target_include_directories(query_generator_test PRIVATE ${WASABI_TEST_INCLUDES})
target_link_libraries(query_generator_test PUBLIC query_generator)
target_link_directories(query_generator_test PUBLIC)

## [main]
add_test(NAME query_generator_test COMMAND query_generator_test)

## [installation]
install(TARGETS query_generator
        LIBRARY DESTINATION ${WASABI_LIBS}
        PUBLIC_HEADER DESTINATION ${WASABI_INCLUDES})

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
