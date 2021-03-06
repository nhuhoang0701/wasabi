message (STATUS "--------------------------------------------------------------")
message (STATUS "-- wasabi.cmake toolchain")

###########################################
# Toolchain file is executed multiple times with unpredictable behavior of reading cached variables,
# this function is used to overcome it
macro(set_temp_var var)
	if(${var})
		# The first time toolchain file is executed, cached variable is read by toolchain file, we
		# save that variable in a temporary env. variable
		set(ENV{_${var}} ${${var}})
	else()
		# In the time that toolchain file doesn't read cached variable, we set that variable again
		# by using temporary env. variable that is set in the first execution of toolchain file
		set(${var} $ENV{_${var}})
	endif()
endmacro()

set_temp_var(WASABI_PLATFORM_TARGET)
set_temp_var(WASMTIME)
set_temp_var(LLVM_DIR)
set_temp_var(SYSROOT_WASI_DIR)
set_temp_var(SYSROOT_LINUX_DIR)


if(DEFINED WASABI_PLATFORM_TARGET)
	message(STATUS "- WASABI_PLATFORM_TARGET='${WASABI_PLATFORM_TARGET}'")
elseif (DEFINED ENV{WASABI_PLATFORM_TARGET})
	message(STATUS "- WASABI_PLATFORM_TARGET=ENV'$ENV{WASABI_PLATFORM_TARGET}' instead WASABI_PLATFORM_TARGET='${WASABI_PLATFORM_TARGET}'")
    set (WASABI_PLATFORM_TARGET "$ENV{WASABI_PLATFORM_TARGET}")
else()
	message(FATAL_ERROR "Missing 'WASABI_PLATFORM_TARGET' definition")
endif()
if(NOT ${WASABI_PLATFORM_TARGET} MATCHES "^(wasm|linux)$")
	message(FATAL_ERROR "${WASABI_PLATFORM_TARGET} should have 'wasm'/'linux' only, WASABI_PLATFORM_TARGET='${WASABI_PLATFORM_TARGET}'")
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
# tools chain / xcompilation / compiler / linker
include(${CMAKE_CURRENT_LIST_DIR}/wasabiVar.cmake)


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
