# Setting necessary variables before executes cmake files in config folder

if("${WASABI_PLATFORM_TARGET}" STREQUAL "wasm")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR wasm)
    set(CLANG_TARGET_TRIPLE wasm32-unknown-wasi)
    message(STATUS "- CLANG_TARGET_TRIPLE='${CLANG_TARGET_TRIPLE}'")

    set(CMAKE_CROSSCOMPILING TRUE)
    set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

    set(CMAKE_C_COMPILER_WORKS TRUE)
    set(CMAKE_C_COMPILER_ID Clang)
    set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

    set(CMAKE_CXX_COMPILER_WORKS TRUE)
    set(CMAKE_CXX_COMPILER_ID Clang)
    set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

	set(CMAKE_CROSSCOMPILING_EMULATOR "${WASMTIME}")
	set(CMAKE_SYSROOT ${SYSROOT_WASI_DIR})
	if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
	endif ()

elseif ("${WASABI_PLATFORM_TARGET}" STREQUAL "linux")
	set(CMAKE_LINKER   ${LLVM_DIR}/bin/clang)
endif()

set(CMAKE_C_COMPILER ${LLVM_DIR}/bin/clang)
set(CMAKE_CXX_COMPILER ${LLVM_DIR}/bin/clang++)

set(CMAKE_AR ${LLVM_DIR}/bin/llvm-ar)
set(CMAKE_C_COMPILER_AR "${CMAKE_AR}")
set(CMAKE_CXX_COMPILER_AR "${CMAKE_AR}")

set(CMAKE_SPLIT ${LLVM_DIR}/bin/llvm-split)

set(CMAKE_RANLIB ${LLVM_DIR}/bin/llvm-ranlib)
set(CMAKE_C_COMPILER_RANLIB "${CMAKE_RANLIB}")
set(CMAKE_CXX_COMPILER_RANLIB "${CMAKE_RANLIB}")

###########################################
# set flags compilator/linker
if ("${WASABI_PLATFORM_TARGET}" STREQUAL "linux")

	#To use llvm linker
	#set(CMAKE_EXE_LINKER_FLAGS    "-fuse-ld=lld")
	#set(CMAKE_SHARED_LINKER_FLAGS    "-fuse-ld=lld")

	#To use llvm libc++
	#set(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} -stdlib=libc++  -I${LLVM_DIR}/include/c++/v1")
	#set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -rtlib=compiler-rt -stdlib=libc++ -L${LLVM_DIR}/lib ${LLVM_DIR}/lib/libunwind.a -L${LLVM_DIR}/lib ${LLVM_DIR}/lib/libc++abi.a")
	#set(CMAKE_SHARED_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -rtlib=compiler-rt -stdlib=libc++ -L${LLVM_DIR}/lib ${LLVM_DIR}/lib/libunwind.a -L${LLVM_DIR}/lib ${LLVM_DIR}/lib/libc++abi.a")

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
endif ()