cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

# CMake json/test

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${WASABI_ROOT_DIR}/src/test_tools/include)

link_directories(${WASABI_ROOT_DIR}/external/cJSON/build)
link_directories(${WASABI_ROOT_DIR}/src/json/build)
link_directories(${WASABI_ROOT_DIR}/src/build/lib)

## [main]

set(SRCS test_json.cpp test_writer.cpp)
set(HEADERS test_data.h)

add_executable(TestJson ${SRCS} ${HEADERS})
target_link_libraries(TestJson PRIVATE cjson jsonUtils exceptions)

## [test]
# run with ctest -V
add_test(NAME TestJson COMMAND TestJson)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
