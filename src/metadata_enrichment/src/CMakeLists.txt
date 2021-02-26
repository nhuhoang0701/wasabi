cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)


# CMake json/src

# [Build the library]



set(SRCS ServerInfo.cpp Tables.cpp)


add_library(metadata_enrichment ${SRCS} ${HEADERS})
include_directories(${WASABI_ROOT_DIR}/external)
include_directories(${PROJECT_SOURCE_DIR}/include)
target_include_directories(
        metadata_enrichment PRIVATE
        ${WASABI_ROOT_DIR}/src/json/include
        ${WASABI_ROOT_DIR}/src/dbproxy
)
target_link_libraries(metadata_enrichment PUBLIC exceptions)
target_link_directories(metadata_enrichment PUBLIC ${WASABI_ROOT_DIR}/src/build/lib/)
## [installation]
install(TARGETS metadata_enrichment DESTINATION ${WASABI_ROOT_DIR}/src/build/lib)
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
