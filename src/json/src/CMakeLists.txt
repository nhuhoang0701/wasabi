cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)


# CMake json/src

# [Build the library]
include_directories(${WASABI_ROOT_DIR}/external)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(SRCS jsonReader.cpp
         jsonWriter.cpp)
set(HEADERS ${PROJECT_SOURCE_DIR}/include/json/jsonReader.h)

add_library(jsonUtils ${SRCS} ${HEADERS})

## [installation]
install(TARGETS jsonUtils DESTINATION ${WASABI_ROOT_DIR}/src/build/lib)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
