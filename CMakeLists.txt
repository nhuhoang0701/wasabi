cmake_minimum_required(VERSION 3.16)

message(STATUS "- WASABI_BUILD_TYPE='${WASABI_BUILD_TYPE}'")
message(STATUS "- WASABI_PLATFORM_TARGET='${WASABI_PLATFORM_TARGET}'")

# Identify operating system on which the current machine is running
if(UNIX AND NOT APPLE)
  set(WASABI_OS "UNIX" CACHE STRING "running os" FORCE)
elseif(APPLE)
  set(WASABI_OS "APPLE" CACHE STRING "running os" FORCE)
endif()

# set wasabi variables which cannot be empty string if they are not undefined
if(NOT DEFINED WASABI_BUILD_TYPE)
    set(WASABI_BUILD_TYPE "Debug")
endif()

if(NOT DEFINED WASABI_PLATFORM_TARGET)
    set(WASABI_PLATFORM_TARGET "wasm")
endif()

# set other variables
set(WASABI_BUILD_DIR_NAME ${CMAKE_SOURCE_DIR}/build/${WASABI_PLATFORM_TARGET}/${WASABI_BUILD_TYPE})
set(WASABI_EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
set(WASABI_INSTALL_DIR ${CMAKE_SOURCE_DIR}/install/${WASABI_PLATFORM_TARGET}/${WASABI_BUILD_TYPE})
set(outfile ${WASABI_EXTERNAL_DIR}/output_ext.log)

# LLVM variables
set(LLVM_VERSION 11.0.0)
if("${WASABI_OS}" STREQUAL "APPLE")
    set(LLVM_COMPRESSED_FILE clang+llvm-${LLVM_VERSION}-x86_64-apple-darwin)
elseif("${WASABI_OS}" STREQUAL "UNIX")

    set(LLVM_COMPRESSED_FILE clang+llvm-${LLVM_VERSION}-x86_64-linux-gnu-ubuntu-20.04)

endif()
set(LLVM_DIR $ENV{LLVM_DIR})

# define wasabi_root_dir
if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()

# Add config and scripts/cmake folder to module search path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/config" "${CMAKE_SOURCE_DIR}/scripts/cmake")

# Get all cmake arguments from command line
get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
  get_property(CACHE_VAR_HELPSTRING CACHE ${CACHE_VAR} PROPERTY HELPSTRING)
  if(CACHE_VAR_HELPSTRING STREQUAL "No help, variable specified on the command line.")
    get_property(CACHE_VAR_TYPE CACHE ${CACHE_VAR} PROPERTY TYPE)
    if(CACHE_VAR_TYPE STREQUAL "UNINITIALIZED")
      set(CACHE_VAR_TYPE)
    else()
      set(CACHE_VAR_TYPE :${CACHE_VAR_TYPE})
    endif()
    list(APPEND WASABI_CLI_VARS -D${CACHE_VAR}${CACHE_VAR_TYPE}=${${CACHE_VAR}})
  endif()
endforeach()
message("WASABI_CLI_VARS: ${WASABI_CLI_VARS}")

# Setup variables
include(cmakeVar)
include(ninjaVar)

# Setup dependencies
include(binaryen)
include(wasmtime)
include(wasmsysroot)
include(sqlite)
include(cjson)

# Link toolchain
include (wasabi)

project(wasabi VERSION 1.0)

#add_subdirectory(external)
add_subdirectory(src)

################################################
#
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
