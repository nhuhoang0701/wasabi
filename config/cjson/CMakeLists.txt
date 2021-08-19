cmake_minimum_required(VERSION 3.16)

project(cjson-install NONE)

set(CJSON_VERSION v1.7.14)

if(NOT EXISTS ${WASABI_EXTERNAL_DIR}/cJSON)
	include(FetchContent)
	FetchContent_Declare(
		cjson
		URL https://github.com/DaveGamble/cJSON/archive/refs/tags/${CJSON_VERSION}.zip
	)

	FetchContent_Populate(cjson)

	message(STATUS ${cjson_SOURCE_DIR})

	# Copy to external folder (optional)
	file(GLOB CJSON_ALL_FILES
			${cjson_SOURCE_DIR}/*)
	file(COPY ${CJSON_ALL_FILES}
			DESTINATION ${WASABI_EXTERNAL_DIR}/cJSON
	)

	set(cjson ${WASABI_EXTERNAL_DIR}/cJSON CACHE PATH "cjson path" FORCE)

	# build cjson step
	execute_process(COMMAND ${CMAKE} -B ${WASABI_BUILD_DIR_NAME}/cjson
							-G Ninja -DCMAKE_MAKE_PROGRAM=${NINJA}
							-Wno-dev
							-DCMAKE_INSTALL_PREFIX=${WASABI_INSTALL_DIR}/cJSON
							-DCMAKE_TOOLCHAIN_FILE=${CMAKE_SOURCE_DIR}/scripts/cmake/wasabi.cmake
							-DCMAKE_C_FLAGS=-fno-stack-protector
							-DENABLE_CJSON_TEST=on
							-DENABLE_CJSON_UTILS=off
							-DBUILD_SHARED_LIBS=off
							-DENABLE_VALGRIND=off
							-DENABLE_SANITIZERS=off
							-DENABLE_CUSTOM_COMPILER_FLAGS=off

					WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR}/cJSON
					RESULT_VARIABLE result_build)

	# Check cJSON build step
	if(result_build)
		execute_process(COMMAND rm -rf cJSON
						WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR})
		message(FATAL_ERROR "Build step for cJSON failed: ${result_build}")
	else()
		message(STATUS "cJSON is built successfully")
	endif()

	# Install cJSON_test target
	execute_process(COMMAND ${CMAKE} --build ${WASABI_BUILD_DIR_NAME}/cjson --target cJSON_test
					WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR}/cJSON
					RESULT_VARIABLE result_install)


	# Check cJSON install step
	if(result_install)
		execute_process(COMMAND rm -rf cJSON
						WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR})
		message(FATAL_ERROR "Install step for cJSON failed: ${result_install}")
	else()
		message(STATUS "cJSON is installed successfully")
	endif()
else()
	message(STATUS "cJSON is already installed")
endif()

# Test file cJSON_test on specific platform
message("Testing cJSON")
execute_process(COMMAND ${WASABI_EXTERNAL_DIR}/wasmtime/wasmtime ${WASABI_INSTALL_DIR}/cJSON/bin/cJSON_test
				WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
				RESULT_VARIABLE cjson_test)

# Check if cJSON test works
if(cjson_test)
	execute_process(COMMAND rm -rf cJSON
					WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR})
	message(FATAL_ERROR "cjson test doesn't work: ${cjson_test}")
else()
	message(STATUS "cjson test passed")
endif()

message(STATUS "--------------------------------------------------------------")
