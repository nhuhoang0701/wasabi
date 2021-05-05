cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(wasabi VERSION 1.0)

#add_subdirectory(external)
add_subdirectory(src)


################################################
# 
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
