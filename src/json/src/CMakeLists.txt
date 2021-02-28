cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

# [Build the library]
set(HEADERS ${PROJECT_SOURCE_DIR}/include/json/jsonReader.h ${PROJECT_SOURCE_DIR}/include/json/jsonWriter.h)
add_library(jsonUtils jsonReader.cpp jsonWriter.cpp)
set_target_properties(jsonUtils PROPERTIES PUBLIC_HEADER "${HEADERS}")

target_include_directories(jsonUtils PRIVATE ../include/ ${WASABI_ROOT_DIR}/external)
target_link_directories(jsonUtils PUBLIC ${WASABI_ROOT_DIR}/external/cJSON/build)
target_link_libraries(jsonUtils PRIVATE cjson exceptions)

## [installation]
install(TARGETS jsonUtils 
		LIBRARY DESTINATION lib
		PUBLIC_HEADER DESTINATION include/json)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
