# Build sqlite
set (SQLITE_COMMAND ${CMAKE} -B ${WASABI_BUILD_DIR_NAME}/sqlite
                        -S ${WASABI_EXTERNAL_DIR}/sqlite
                        -G Ninja
                        -DCMAKE_MAKE_PROGRAM=${NINJA}
                        -Wno-dev
                        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_SOURCE_DIR}/scripts/cmake/wasabi.cmake
                        -DCMAKE_C_FLAGS=-fno-stack-protector)
list(APPEND SQLITE_COMMAND ${WASABI_CLI_VARS})
# Override CMAKE_INSTALL_PREFIX of sqlite over default one passed in command
list(APPEND SQLITE_COMMAND -DCMAKE_INSTALL_PREFIX=${WASABI_INSTALL_DIR}/sqlite)

execute_process(COMMAND ${SQLITE_COMMAND}
                WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR}/sqlite
                RESULT_VARIABLE result_build)


# Check result of build step
if(result_build)
	execute_process(COMMAND rm -rf build
					WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR}/sqlite)
    message(FATAL_ERROR "Build step for sqlite failed: ${result_build}")
else()
	message("sqlite is built successfully")
endif()

# Install sqlite
execute_process(COMMAND ${CMAKE} --build ${WASABI_BUILD_DIR_NAME}/sqlite --target install
                WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR}/sqlite
                RESULT_VARIABLE result_install)


# Check result of install step
if(result_install)
	execute_process(COMMAND rm -rf build
					WORKING_DIRECTORY ${WASABI_EXTERNAL_DIR}/sqlite)
    message(FATAL_ERROR "Install step for sqlite failed: ${result_install}")
else()
	message("sqlite is installed successfully")
endif()
