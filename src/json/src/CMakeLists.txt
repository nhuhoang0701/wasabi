cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)


project(jsonUtils VERSION 1.0 LANGUAGES CXX)

# CMake json/src

# [Build the library]
include_directories(${WASABI_ROOT_DIR}/external)
include_directories(${PROJECT_SOURCE_DIR}/../include)

set(SRCS jsonReader.cpp
         jsonWriter.cpp)
set(HEADERS ${PROJECT_SOURCE_DIR}/../include/json/jsonReader.h ${PROJECT_SOURCE_DIR}/../include/json/jsonWriter.h)

add_library(jsonUtils ${SRCS} ${HEADERS})
set_target_properties(jsonUtils PROPERTIES PUBLIC_HEADER "${HEADERS}")

## [installation]
install(TARGETS jsonUtils 
			LIBRARY DESTINATION lib
			PUBLIC_HEADER DESTINATION include)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
