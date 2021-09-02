cmake_minimum_required(VERSION 3.16)


set(WASMTIME_VERSION v0.22.0)

# Get wasmtime file which is compatible to the running OS
if ("${WASABI_OS}" STREQUAL "UNIX")
    set(WASMTIME_COMPRESSED_FILE wasmtime-${WASMTIME_VERSION}-x86_64-linux)
elseif("${WASABI_OS}" STREQUAL "APPLE")
    set(WASMTIME_COMPRESSED_FILE wasmtime-${WASMTIME_VERSION}-x86_64-macos)
endif()


# Download wasmtime if not already installed
if (NOT EXISTS ${WASABI_EXTERNAL_DIR}/wasmtime/wasmtime)
    include(FetchContent)
    FetchContent_Declare(
        wasmtime
        URL https://github.com/bytecodealliance/wasmtime/releases/download/${WASMTIME_VERSION}/${WASMTIME_COMPRESSED_FILE}.tar.xz
        DOWNLOAD_NO_EXTRACT TRUE
        DOWNLOAD_DIR ${WASABI_EXTERNAL_DIR}
        DOWNLOAD_NAME wasmtime.tar.xz
    )

    FetchContent_Populate(wasmtime)

# Extract in external folder
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar -Jfx "${WASABI_EXTERNAL_DIR}/wasmtime.tar.xz"
                    WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR})

    execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${WASMTIME_COMPRESSED_FILE} "wasmtime"
COMMAND ${CMAKE_COMMAND} -E rm -f wasmtime.tar.xz
                    WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR})


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
