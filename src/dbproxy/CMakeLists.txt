cmake_minimum_required(VERSION 3.20)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)


project(
  dbproxy
  VERSION 1.0
  LANGUAGES CXX)

include_directories(${WASABI_ROOT_DIR}/src/test_tools/include)
link_directories(${WASABI_ROOT_DIR}/src/build/lib)

add_library(dbproxy dbproxy.cpp)

add_executable(dbproxy_test dbproxy_test.cpp)
target_link_libraries(dbproxy_test dbproxy exceptions.wasm)

## [main]
enable_testing()
add_test(NAME dbproxy_test COMMAND dbproxy_test)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
