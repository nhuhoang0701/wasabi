cmake_minimum_required(VERSION 3.16)

message(STATUS "- WASABI_BUILD_TYPE='${WASABI_BUILD_TYPE}'")
message(STATUS "- WASABI_PLATFORM_TARGET='${WASABI_PLATFORM_TARGET}'")

# Identify operating system on which the current machine is running
if(UNIX AND NOT APPLE)
  set(WASABI_OS "UNIX" CACHE STRING "running os" FORCE)
elseif(APPLE)
  set(WASABI_OS "APPLE" CACHE STRING "running os" FORCE)
endif()

# wasabi variables
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

# Create necessary cmake variables
include(${CMAKE_SOURCE_DIR}/scripts/cmake/wasabiVar.cmake)

project(wasabi VERSION 1.0)

# Download dependencies
add_subdirectory(config)

# Link toolchain
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

#add_subdirectory(external)
add_subdirectory(src)

################################################
#
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
