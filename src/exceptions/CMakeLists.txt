cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)


project(
  exceptions
  VERSION 1.0
  LANGUAGES CXX)

if ("${WASABI_PLATFORM_TARGET}" STREQUAL "wasm")
add_library(exceptions cxa_exception.cpp)
else ()
add_library(exceptions cxa_exception_empty.cpp)
endif()

install(TARGETS exceptions DESTINATION lib)
#target_link_options( exceptions PUBLIC LINKER:--export=enableThrowJSException,--allow-undefined-file=${WASABI_ROOT_DIR}/src/exceptions/js_fcts.syms)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
