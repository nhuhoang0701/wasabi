cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

# CMake metadata_enrichment/test

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${WASABI_ROOT_DIR}/src/test_tools/include)
link_directories(${WASABI_ROOT_DIR}/external/cJSON/build)
link_directories(${WASABI_ROOT_DIR}/src/json/build)
link_directories(${WASABI_ROOT_DIR}/src/build/lib)

## [main]

set(SRCS metadata_enrichment_test.cpp
         ServerInfo_test.cpp Tables_test.cpp
   )
set(HEADERS metadata_enrichtment_test.h)

add_executable(TestMetaData ${SRCS} ${HEADERS})
target_include_directories(
  TestMetaData PRIVATE
    ${WASABI_ROOT_DIR}/src/metadata_enrichment/include
    ${WASABI_ROOT_DIR}/src/dbproxy
    ${WASABI_ROOT_DIR}/src/json/include
)
target_link_libraries(
   TestMetaData PRIVATE
        metadata_enrichment
        cjson
        jsonUtils
        dbproxy
        exceptions.wasm
 )

## [test]
# run with ctest -V
add_test(NAME TestMetaData COMMAND TestMetaData)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
