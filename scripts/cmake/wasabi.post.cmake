message (TRACE "--------------------------------------------------------------")
message (TRACE "-- wasabi.post.cmake toolchain")

if ("${WASABI_PLATFORM_TARGET}" STREQUAL "wasm")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
	#set(CMAKE_SHARED_LIBRARY_SUFFIX ".wasm.so")
	#set(CMAKE_STATIC_LIBRARY_SUFFIX ".wasm.a")
	#set(CMAKE_IMPORT_LIBRARY_SUFFIX ".wasm.a")
endif ()

message (TRACE "--------------------------------------------------------------")