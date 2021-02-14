cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)


project(
  Exceptions
  VERSION 1.0
  LANGUAGES CXX)

add_library(exceptions cxa_exception.cpp)

install(TARGETS exceptions DESTINATION ${WASABI_ROOT_DIR}/src/build/lib)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
