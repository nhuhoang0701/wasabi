cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(wasabi_src VERSION 1.0)


add_subdirectory(exceptions)

add_subdirectory(json)

add_subdirectory(dbproxy)
add_subdirectory(InA_query_model)
#add_subdirectory(query_generator)
add_subdirectory(cube)
add_subdirectory(grid)
add_subdirectory(metadata_enrichment)
#add_subdirectory(InA_Interpreter)


add_subdirectory(test)


################################################
# 
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
