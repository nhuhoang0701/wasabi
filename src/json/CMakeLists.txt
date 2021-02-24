cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  JsonUtils
  VERSION 1.0
  LANGUAGES CXX)

add_subdirectory(src)

add_subdirectory(test)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
