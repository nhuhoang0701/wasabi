cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(wasi_browser VERSION 1.0 LANGUAGES CXX)

# [merge JS in one]
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/WASI_API.js WASI_API_JS)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/Microcube_worker.js MICROCUBE_WORKER_JS)
string(REPLACE "importScripts('./WASI_API.js');" "${WASI_API_JS}" MICROCUBE_WORKER_JS_WASI_API_JS "${MICROCUBE_WORKER_JS}")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Microcube_worker_WASI.js "${MICROCUBE_WORKER_JS_WASI_API_JS}")

## [test]
# run with ctest -V

## [installation]
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Microcube_worker_WASI.js
        DESTINATION . RENAME Microcube_worker.js)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/WASI_API.js    DESTINATION . )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/WASI_Module.js DESTINATION . )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.js        DESTINATION . )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/index.html     DESTINATION . )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/favicon.ico     DESTINATION . )

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
