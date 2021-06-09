message (STATUS "--------------------------------------------------------------")
message (STATUS "-- wasabi.cmake toolchain")

###########################################
# Variables
if(DEFINED WASABI_PLATFORM_TARGET)
	message(STATUS "- WASABI_PLATFORM_TARGET='${WASABI_PLATFORM_TARGET}'")
elseif (DEFINED ENV{WASABI_PLATFORM_TARGET})
	message(STATUS "- WASABI_PLATFORM_TARGET=ENV'$ENV{WASABI_PLATFORM_TARGET}' instead WASABI_PLATFORM_TARGET='${WASABI_PLATFORM_TARGET}'")
    set (WASABI_PLATFORM_TARGET "$ENV{WASABI_PLATFORM_TARGET}")
else()
	message(FATAL_ERROR "Missing 'WASABI_PLATFORM_TARGET' definition")
endif()

if(DEFINED WASMTIME)
	message(STATUS "- WASMTIME='${WASMTIME}'")
elseif (DEFINED ENV{WASMTIME})
	message(STATUS "- WASMTIME=ENV'$ENV{WASMTIME}' instead WASMTIME='${WASMTIME}'")
    set (WASMTIME "$ENV{WASMTIME}")
else()
	message(FATAL_ERROR "Missing 'WASMTIME' definition")
endif()

if(DEFINED SYSROOT_WASI_DIR)
	message(STATUS "- SYSROOT_WASI_DIR='${SYSROOT_WASI_DIR}'")
elseif (DEFINED ENV{SYSROOT_WASI_DIR})
	message(STATUS "- SYSROOT_WASI_DIR=ENV'$ENV{SYSROOT_WASI_DIR}' instead SYSROOT_WASI_DIR='${SYSROOT_WASI_DIR}'")
    set (SYSROOT_WASI_DIR "$ENV{SYSROOT_WASI_DIR}")
else()
	message(FATAL_ERROR "Missing 'SYSROOT_WASI_DIR' definition")
endif()
if(DEFINED SYSROOT_LINUX_DIR)
	message(STATUS "- SYSROOT_LINUX_DIR='${SYSROOT_LINUX_DIR}'")
elseif (DEFINED ENV{SYSROOT_LINUX_DIR})
	message(STATUS "- SYSROOT_LINUX_DIR=ENV'$ENV{SYSROOT_LINUX_DIR}' instead SYSROOT_LINUX_DIR='${SYSROOT_LINUX_DIR}'")
    set (SYSROOT_LINUX_DIR "$ENV{SYSROOT_LINUX_DIR}")
else()
	message(STATUS "Missing 'SYSROOT_LINUX_DIR' definition")
endif()

if(DEFINED LLVM_DIR)
	message(STATUS "- LLVM_DIR='${LLVM_DIR}'")
elseif (DEFINED ENV{LLVM_DIR})
	message(STATUS "- LLVM_DIR=ENV'$ENV{LLVM_DIR}' instead LLVM_DIR='${LLVM_DIR}'")
    set (LLVM_DIR "$ENV{LLVM_DIR}")
else()
	message(FATAL_ERROR "Missing 'LLVM_DIR' definition")
endif()

if(DEFINED NINJA)
	message(STATUS "- NINJA='${NINJA}'")
elseif (DEFINED ENV{NINJA})
	message(STATUS "- NINJA=ENV'$ENV{NINJA}' instead NINJA='${NINJA}'")
    set (NINJA "$ENV{NINJA}")
else()
	message(STATUS "Missing 'NINJA' definition")
endif()

###########################################
# tools chain / xcompilation
if ("${WASABI_PLATFORM_TARGET}" STREQUAL "wasm")
	set(CMAKE_SYSTEM_NAME Generic)
	set(CMAKE_SYSTEM_PROCESSOR wasm)
	set(CLANG_TARGET_TRIPLE wasm32-unknown-wasi)
	message(STATUS "- CLANG_TARGET_TRIPLE='${CLANG_TARGET_TRIPLE}'")

	set(CMAKE_CROSSCOMPILING TRUE)
	set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
	set(CMAKE_CROSSCOMPILING_EMULATOR "${WASMTIME}")

	set(CMAKE_SYSROOT ${SYSROOT_WASI_DIR})

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
	set(CMAKE_C_COMPILER_AR "${CMAKE_AR}")
	set(CMAKE_CXX_COMPILER_AR "${CMAKE_AR}")
	set(CMAKE_C_COMPILER_RANLIB "${CMAKE_RANLIB}")
	set(CMAKE_CXX_COMPILER_RANLIB "${CMAKE_RANLIB}")

	if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
	endif ()

elseif ("${WASABI_PLATFORM_TARGET}" STREQUAL "linux")
	set(CMAKE_C_COMPILER ${LLVM_DIR}/bin/clang)
	set(CMAKE_CXX_COMPILER ${LLVM_DIR}/bin/clang++)
	set(CMAKE_LINKER   ${LLVM_DIR}/bin/clang)
	set(CMAKE_AR ${LLVM_DIR}/bin/llvm-ar)
	set(CMAKE_RANLIB ${LLVM_DIR}/bin/llvm-ranlib)
	set(CMAKE_SPLIT ${LLVM_DIR}/bin/llvm-split)
	set(CMAKE_C_COMPILER_AR "${CMAKE_AR}")
	set(CMAKE_CXX_COMPILER_AR "${CMAKE_AR}")
	set(CMAKE_C_COMPILER_RANLIB "${CMAKE_RANLIB}")
	set(CMAKE_CXX_COMPILER_RANLIB "${CMAKE_RANLIB}")
	
	set(CMAKE_EXE_LINKER_FLAGS    "-fuse-ld=lld")
	set(CMAKE_SHARED_LINKER_FLAGS    "-fuse-ld=lld")

	set(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} -stdlib=libc++  -I${LLVM_DIR}/include/c++/v1")
	set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -rtlib=compiler-rt -stdlib=libc++ -L${LLVM_DIR}/lib ${LLVM_DIR}/lib/libunwind.a -L${LLVM_DIR}/lib ${LLVM_DIR}/lib/libc++abi.a")
	set(CMAKE_SHARED_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -rtlib=compiler-rt -stdlib=libc++ -L${LLVM_DIR}/lib ${LLVM_DIR}/lib/libunwind.a -L${LLVM_DIR}/lib ${LLVM_DIR}/lib/libc++abi.a")

	set(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} -gsplit-dwarf")
	set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -gsplit-dwarf")
	set(CMAKE_SHARED_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -gsplit-dwarf")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
	set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
	set(CMAKE_SHARED_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

	set(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} -fsanitize=address")
	set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
	set(CMAKE_SHARED_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
#	set(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} -fsanitize=memory")
#	set(CMAKE_SHARED_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")
#	set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")
endif()

#TODO: use musl as static libc
#	set(CLANG_TARGET_TRIPLE x86_64-linux-musl)
#	message(STATUS "- CLANG_TARGET_TRIPLE='${CLANG_TARGET_TRIPLE}'")
	
#	set(CMAKE_SYSROOT ${SYSROOT_LINUX_DIR})
	
#	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,-rpath,${LLVM_DIR}/lib")
#	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath,${LLVM_DIR}/lib")
#	set(CMAKE_STATIC_LINKER_FLAGS "")
#	set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,${LLVM_DIR}/lib -Xlinker --allow-multiple-definition")
else ()
	message(FATAL_ERROR "${WASABI_PLATFORM_TARGET} should have 'wasm'/'linux' only, WASABI_PLATFORM_TARGET='${WASABI_PLATFORM_TARGET}'")
endif ()

###########################################
# compilation option
message(STATUS "- CMAKE_BUILD_TYPE='${CMAKE_BUILD_TYPE}'")
  
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#add_compile_options("-Wall")
add_compile_options("-Werror")


###########################################
# wasabi external
set(cJSON_LIBRARY_DIRS ${CMAKE_INSTALL_PREFIX}/cJSON/lib)
set(SQLITE_LIBRARY_DIRS ${CMAKE_INSTALL_PREFIX}/sqlite/lib)
set(SQLITE_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/sqlite/include)

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

message (STATUS "--------------------------------------------------------------")
