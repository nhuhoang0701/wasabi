message (TRACE "--------------------------------------------------------------")
message (TRACE "-- wasabi.cmake toolchain")

###########################################
# Variables
if(DEFINED WASABI_USE_WASM)
	message(TRACE "- WASABI_USE_WASM='${WASABI_USE_WASM}'")
elseif (DEFINED ENV{WASABI_USE_WASM})
	message(TRACE "- WASABI_USE_WASM=ENV'$ENV{WASABI_USE_WASM}' instead WASABI_USE_WASM='${WASABI_USE_WASM}'")
    set (WASABI_USE_WASM "$ENV{WASABI_USE_WASM}" CACHE PATH "project root" FORCE)
else()
	message(FATAL_ERROR "Missing 'WASABI_USE_WASM' definition")
endif()

if(DEFINED WASMTIME)
	message(TRACE "- WASMTIME='${WASMTIME}'")
elseif (DEFINED ENV{WASMTIME})
	message(TRACE "- WASMTIME=ENV'$ENV{WASMTIME}' instead WASMTIME='${WASMTIME}'")
    set (WASMTIME "$ENV{WASMTIME}" CACHE PATH "project root" FORCE)
else()
	message(FATAL_ERROR "Missing 'WASMTIME' definition")
endif()

if(DEFINED SYSROOT_DIR)
	message(TRACE "- SYSROOT_DIR='${SYSROOT_DIR}'")
elseif (DEFINED ENV{SYSROOT_DIR})
	message(TRACE "- SYSROOT_DIR=ENV'$ENV{SYSROOT_DIR}' instead SYSROOT_DIR='${SYSROOT_DIR}'")
    set (SYSROOT_DIR "$ENV{SYSROOT_DIR}" CACHE PATH "project root" FORCE)
else()
	message(FATAL_ERROR "Missing 'SYSROOT_DIR' definition")
endif()

if(DEFINED LLVM_DIR)
	message(TRACE "- LLVM_DIR='${LLVM_DIR}'")
elseif (DEFINED ENV{LLVM_DIR})
	message(TRACE "- LLVM_DIR=ENV'$ENV{LLVM_DIR}' instead LLVM_DIR='${LLVM_DIR}'")
    set (LLVM_DIR "$ENV{LLVM_DIR}" CACHE PATH "project root" FORCE)
else()
	message(FATAL_ERROR "Missing 'LLVM_DIR' definition")
endif()

if(DEFINED NINJA)
	message(TRACE "- NINJA='${NINJA}'")
elseif (DEFINED ENV{NINJA})
	message(TRACE "- NINJA=ENV'$ENV{NINJA}' instead NINJA='${NINJA}'")
    set (NINJA "$ENV{NINJA}" CACHE PATH "project root" FORCE)
else()
	message(TRACE "Missing 'NINJA' definition")
endif()


###########################################
# tools chain / xcompilation
if ("${WASABI_USE_WASM}" STREQUAL "yes")
	set(CMAKE_SYSTEM_NAME Generic)
	set(CMAKE_SYSTEM_PROCESSOR wasm)
	set(CLANG_TARGET_TRIPLE wasm32-unknown-wasi)
	message(TRACE "- CLANG_TARGET_TRIPLE='${CLANG_TARGET_TRIPLE}'")

	set(CMAKE_CROSSCOMPILING TRUE)
	set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
	set(CMAKE_CROSSCOMPILING_EMULATOR "${WASMTIME}")

	set(CMAKE_SYSROOT ${SYSROOT_DIR})

	set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
	set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

	set(CMAKE_C_COMPILER_WORKS TRUE)
	set(CMAKE_C_COMPILER_ID Clang)
	set(CMAKE_C_COMPILER ${LLVM_DIR}/bin/clang)
	set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

	set(CMAKE_CXX_COMPILER_WORKS TRUE)
	set(CMAKE_CXX_COMPILER_ID Clang)
	set(CMAKE_CXX_COMPILER ${LLVM_DIR}/bin/clang++)
	set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

	set(CMAKE_AR ${LLVM_DIR}/bin/llvm-ar)
	set(CMAKE_RANLIB ${LLVM_DIR}/bin/llvm-ranlib)
	set(CMAKE_SPLIT ${LLVM_DIR}/bin/llvm-split)
elseif ("${WASABI_USE_WASM}" STREQUAL "no")
	set(CMAKE_C_COMPILER ${LLVM_DIR}/bin/clang)
	set(CMAKE_CXX_COMPILER ${LLVM_DIR}/bin/clang++)
	set(CMAKE_LINKER   ${LLVM_DIR}/bin/llvm-ld)
	set(CMAKE_AR ${LLVM_DIR}/bin/llvm-ar)
	set(CMAKE_RANLIB ${LLVM_DIR}/bin/llvm-ranlib)
	set(CMAKE_SPLIT ${LLVM_DIR}/bin/llvm-split)
	set(CMAKE_CXX_FLAGS "-I${LLVM_DIR}/include/c++/v1")
	set(CMAKE_MODULE_LINKER_FLAGS "-stdlib=libc++ -nostdinc++ -L${LLVM_DIR}/lib -Wl,-rpath,${LLVM_DIR}/lib")
	set(CMAKE_SHARED_LINKER_FLAGS "-stdlib=libc++ -nostdinc++ -L${LLVM_DIR}/lib -Wl,-rpath,${LLVM_DIR}/lib")
	set(CMAKE_STATIC_LINKER_FLAGS "")
	#set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++ -nostdinc++  -L${LLVM_DIR}/lib -Wl,-rpath,${LLVM_DIR}/lib -Xlinker --allow-multiple-definition")
else ()
	message(FATAL_ERROR "${WASABI_USE_WASM} should have 'yes'/'no' only, WASABI_USE_WASM='${WASABI_USE_WASM}'")
endif ()

###########################################
# compilation option
set(CMAKE_BUILD_TYPE Debug)
message(TRACE "- CMAKE_BUILD_TYPE='${CMAKE_BUILD_TYPE}'")
  
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#For none debug
#set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
#add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fwasm-exceptions>)


###########################################
# wasabi external
set(cJSON_LIBRARY_DIRS ${WASABI_ROOT_DIR}/external/cJSON/install/lib)

###########################################
# wasabi var
set(WASABI_INCLUDES ${CMAKE_INSTALL_PREFIX}/include)
set(WASABI_LIBS ${CMAKE_INSTALL_PREFIX}/lib)

include_directories(${WASABI_ROOT_DIR}/src)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)

###########################################
# cmake option
enable_testing()

message (TRACE "--------------------------------------------------------------")
