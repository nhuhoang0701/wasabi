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
    )

    FetchContent_Populate(binaryen)

    message(STATUS "Binaryen source directory downloaded from git: " ${binaryen_SOURCE_DIR})

    # Copy to external folder (optional)
    file(GLOB BINARYEN_ALL_FILES
            ${binaryen_SOURCE_DIR}/*)
    file(COPY ${BINARYEN_ALL_FILES}
            DESTINATION ${WASABI_EXTERNAL_DIR}/binaryen/install
    )
    message(STATUS "New binaryen directory: " ${WASABI_EXTERNAL_DIR}/binaryen/install)
    set(BINARYEN_DIR ${WASABI_EXTERNAL_DIR}/binaryen CACHE PATH "binaryen path" FORCE)
else()
    message(STATUS "Binaryen is already installed in " ${WASABI_EXTERNAL_DIR}/binaryen/install)
endif()

message(STATUS "--------------------------------------------------------------")
