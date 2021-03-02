cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(json VERSION 1.0 LANGUAGES CXX)

# [Build the library]
set(HEADERS jsonReader.h jsonWriter.h)
add_library(json jsonReader.cpp jsonWriter.cpp)
set_target_properties(json PROPERTIES PUBLIC_HEADER "${HEADERS}")

target_include_directories(json PRIVATE ../include/ ${WASABI_ROOT_DIR}/external)
target_link_directories(json PUBLIC ${WASABI_ROOT_DIR}/external/cJSON/build ${WASABI_LIBS})
target_link_libraries(json PRIVATE cjson exceptions)

## [main]
add_executable(test_reader test_reader.cpp)
target_include_directories(test_reader PRIVATE ${WASABI_INCLUDES} ${WASABI_ROOT_DIR}/src/test_tools/include)
target_link_libraries(test_reader PRIVATE cjson json exceptions)

add_executable(test_writer test_writer.cpp)
target_include_directories(test_writer PRIVATE ${WASABI_INCLUDES} ${WASABI_ROOT_DIR}/src/test_tools/include)
target_link_libraries(test_writer PRIVATE cjson json exceptions)

## [test]
# run with ctest -V
add_test(NAME test_reader COMMAND test_reader)
add_test(NAME test_writer COMMAND test_writer)

## [installation]
install(TARGETS json 
		LIBRARY DESTINATION lib
		PUBLIC_HEADER DESTINATION include/json)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
