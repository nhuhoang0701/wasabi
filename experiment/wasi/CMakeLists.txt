cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)


project(
  ModernCMakeExample
  VERSION 1.0
  LANGUAGES CXX)

add_library(MyLibExample simple_lib.cpp simple_lib.hpp)
add_executable(MyExample simple_example.cpp)
target_link_libraries(MyExample PRIVATE MyLibExample)

add_test(NAME MyExample COMMAND MyExample)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)

