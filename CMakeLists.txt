cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(InA_query_model VERSION 1.0)




add_subdirectory(src/exceptions)

add_subdirectory(src/json)

add_subdirectory(src/dbproxy)
add_subdirectory(src/InA_query_model)
#add_subdirectory(src/query_generator)
add_subdirectory(src/cube)
add_subdirectory(src/grid)
add_subdirectory(src/metadata_enrichment)
add_subdirectory(src/InA_Interpreter)




################################################
# 
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
