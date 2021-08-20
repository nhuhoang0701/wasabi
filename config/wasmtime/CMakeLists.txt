cmake_minimum_required(VERSION 3.16)


set(WASMTIME_VERSION v0.22.0)

# Get wasmtime file which is compatible to the running OS
if ("${WASABI_OS}" STREQUAL "UNIX")
    set(WASMTIME_COMPRESSED_FILE wasmtime-${WASMTIME_VERSION}-x86_64-linux)
elseif("${WASABI_OS}" STREQUAL "APPLE")
    set(WASMTIME_COMPRESSED_FILE wasmtime-${WASMTIME_VERSION}-x86_64-macos)
endif()

# Set environment variable
set(ENV{WASMTIME} ${WASABI_EXTERNAL_DIR}/wasmtime/wasmtime)
message(STATUS "WASMTIME=$ENV{WASMTIME}")

# Download wasmtime if not already installed
if (NOT EXISTS ${WASABI_EXTERNAL_DIR}/wasmtime/wasmtime)
    include(FetchContent)
    FetchContent_Declare(
        wasmtime
        URL https://github.com/bytecodealliance/wasmtime/releases/download/${WASMTIME_VERSION}/${WASMTIME_COMPRESSED_FILE}.tar.xz
    )

    FetchContent_Populate(wasmtime)


    file(GLOB WASMTIME_ALL_FILES
            ${wasmtime_SOURCE_DIR}/*)
    file(COPY ${WASMTIME_ALL_FILES}
            DESTINATION ${WASABI_EXTERNAL_DIR}/wasmtime
    )

    if (EXISTS ${WASABI_EXTERNAL_DIR}/wasmtime/wasmtime)
        message(STATUS "New wasmtime path: " ${WASABI_EXTERNAL_DIR}/wasmtime/wasmtime)
    else()
        execute_process(COMMAND rm -rf wasmtime
                        WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR})
        message(FATAL_ERROR "Install step wasmtime failed")
    endif()

    message(STATUS "Wasmtime is installed successfully in " ${WASABI_EXTERNAL_DIR}/wasmtime)

else()
    message(STATUS "Wasmtime is already installed in " ${WASABI_EXTERNAL_DIR}/wasmtime)
endif()

message(STATUS "--------------------------------------------------------------")
