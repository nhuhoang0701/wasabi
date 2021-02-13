#**************************************************************
if(DEFINED WASMTIME)
	message("WASMTIME='${WASMTIME}'")
elseif (DEFINED ENV{WASMTIME})
	message("use WASMTIME=ENV'$ENV{WASMTIME}' instead WASMTIME='${WASMTIME}'")
    set (WASMTIME "$ENV{WASMTIME}" CACHE PATH "project root" FORCE)
else()
	message(FATAL_ERROR "Missing 'WASMTIME' definition")
endif()


#
if(DEFINED SYSROOT_DIR)
	message("SYSROOT_DIR='${SYSROOT_DIR}'")
elseif (DEFINED ENV{SYSROOT_DIR})
	message("use SYSROOT_DIR=ENV'$ENV{SYSROOT_DIR}' instead SYSROOT_DIR='${SYSROOT_DIR}'")
    set (SYSROOT_DIR "$ENV{SYSROOT_DIR}" CACHE PATH "project root" FORCE)
else()
	message(FATAL_ERROR "Missing 'SYSROOT_DIR' definition")
endif()

#
if(DEFINED LLVM_DIR)
	message("LLVM_DIR='${LLVM_DIR}'")
elseif (DEFINED ENV{LLVM_DIR})
	message("use LLVM_DIR=ENV'$ENV{LLVM_DIR}' instead LLVM_DIR='${LLVM_DIR}'")
    set (LLVM_DIR "$ENV{LLVM_DIR}" CACHE PATH "project root" FORCE)
else()
	message(FATAL_ERROR "Missing 'LLVM_DIR' definition")
endif()

#**************************************************************

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_CROSSCOMPILING TRUE)
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
set(CMAKE_CROSSCOMPILING_EMULATOR "${WASMTIME}")

set(CMAKE_SYSTEM_PROCESSOR wasm)

set(CMAKE_C_COMPILER_ID_RUN TRUE)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_C_COMPILER_ID Clang)

set(CMAKE_CXX_COMPILER_ID_RUN TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_ID Clang)

set(CMAKE_SYSROOT ${SYSROOT_DIR})
set(CLANG_TARGET_TRIPLE wasm32-unknown-wasi)

set(CMAKE_C_COMPILER ${LLVM_DIR}/bin/clang)
set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_CXX_COMPILER ${LLVM_DIR}/bin/clang++)
set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

set(CMAKE_AR ${LLVM_DIR}/bin/llvm-ar)
set(CMAKE_SPLIT ${LLVM_DIR}/bin/llvm-split)	

set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
