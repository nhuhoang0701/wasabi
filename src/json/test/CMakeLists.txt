cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

# CMake json/test
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${WASABI_ROOT_DIR}/external/cJSON/build)

## [main]
add_executable(TestJson test_json.cpp)
target_include_directories(TestJson PRIVATE ${WASABI_INCLUDES} ${WASABI_ROOT_DIR}/src/test_tools/include)
target_link_libraries(TestJson PRIVATE cjson jsonUtils exceptions)

add_executable(test_writer test_writer.cpp)
target_include_directories(test_writer PRIVATE ${WASABI_INCLUDES} ${WASABI_ROOT_DIR}/src/test_tools/include)
target_link_libraries(test_writer PRIVATE cjson jsonUtils exceptions)

## [test]
# run with ctest -V
add_test(NAME TestJson COMMAND TestJson)
add_test(NAME test_writer COMMAND test_writer)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
