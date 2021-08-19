cmake_minimum_required(VERSION 3.16)

project(cmake-set-vars NONE)

set(WASABI_CMAKE_DIR ${WASABI_EXTERNAL_DIR}/cmake CACHE PATH "Path of local cmake directory" FORCE)
message(TRACE "WASABI_CMAKE_DIR=" ${WASABI_CMAKE_DIR})
set(CMAKE ${WASABI_CMAKE_DIR}/bin/cmake CACHE PATH "Path of local binary cmake file" FORCE)
message(TRACE "CMAKE=" ${CMAKE})
set(CTEST ${WASABI_CMAKE_DIR}/bin/ctest CACHE PATH "Path of local binary ctest file" FORCE)
message(TRACE "CTEST=" ${CTEST})

message(STATUS "--------------------------------------------------------------")
