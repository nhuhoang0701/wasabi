cmake_minimum_required(VERSION 3.16)

project(binaryen NONE)

set(BINARYEN_VERSION version_100)

include(FetchContent)

# Identify OS on which Wasabi is running
if ("${WASABI_OS}" STREQUAL "UNIX")
    set(BINARYEN_COMPRESSED_FILE binaryen-${BINARYEN_VERSION}-x86_64-linux)
elseif("${WASABI_OS}" STREQUAL "APPLE")
    set(BINARYEN_COMPRESSED_FILE binaryen-${BINARYEN_VERSION}-x86_64-macos)
endif()

if(NOT EXISTS ${WASABI_EXTERNAL_DIR}/binaryen/install)
    # Download repo from git
    FetchContent_Declare(
        binaryen
        URL https://github.com/WebAssembly/binaryen/releases/download/${BINARYEN_VERSION}/${BINARYEN_COMPRESSED_FILE}.tar.gz
        DOWNLOAD_NO_EXTRACT TRUE
        DOWNLOAD_DIR ${WASABI_EXTERNAL_DIR}/binaryen
        DOWNLOAD_NAME binaryen.tar.gz
    )

    FetchContent_Populate(binaryen)

    message(STATUS "Binaryen source directory downloaded from git: " ${binaryen_SOURCE_DIR})

    # Copy to external folder (optional)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar -Jfx "${WASABI_EXTERNAL_DIR}/binaryen/binaryen.tar.gz"
                    WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR}/binaryen)

    execute_process( COMMAND ${CMAKE_COMMAND} -E rename "binaryen-${BINARYEN_VERSION}" "install"
                    WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR}/binaryen)

    message(STATUS "New binaryen directory: " ${WASABI_EXTERNAL_DIR}/binaryen/install)
    set(BINARYEN_DIR ${WASABI_EXTERNAL_DIR}/binaryen CACHE PATH "binaryen path" FORCE)
else()
    message(STATUS "Binaryen is already installed in " ${WASABI_EXTERNAL_DIR}/binaryen/install)
endif()

message(STATUS "--------------------------------------------------------------")
