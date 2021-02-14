message (TRACE "--------------------------------------------------------------")
message (TRACE "-- wasabi.post.cmake toolchain")

set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
message(TRACE "- CMAKE_EXECUTABLE_SUFFIX='${CMAKE_EXECUTABLE_SUFFIX}'")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".wasm.a")

message (TRACE "--------------------------------------------------------------")