cmake_minimum_required(VERSION 3.16)


set(WASISDK_VERSION 12)

# Set environment variable for SYSROOT_WASI_DIR
set(ENV{SYSROOT_WASI_DIR} ${WASABI_EXTERNAL_DIR}/sysroot/wasi-sysroot)
message(STATUS "SYSROOT_WASI_DIR=$ENV{SYSROOT_WASI_DIR}")

if (NOT EXISTS ${WASABI_EXTERNAL_DIR}/sysroot/wasi-sysroot)
    include(FetchContent)

    FetchContent_Declare(
    wasisdk
            URL https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${WASISDK_VERSION}/wasi-sysroot-${WASISDK_VERSION}.0.tar.gz
    )

    FetchContent_Populate(wasisdk)

    # Copy to external folder (optional)
    file(GLOB WASISDK_ALL_FILES
            ${wasisdk_SOURCE_DIR}/*)
    file(COPY ${WASISDK_ALL_FILES}
            DESTINATION ${WASABI_EXTERNAL_DIR}/sysroot/wasi-sysroot)

    message(STATUS "New wasisdk directory: " ${WASABI_EXTERNAL_DIR}/sysroot/wasi-sysroot)

else()
    message(STATUS "wasisdk is already installed in " ${WASABI_EXTERNAL_DIR}/sysroot/wasi-sysroot)
endif()

# Install library libclang_rt.builtins-wasm32-wasi into clang folder
if(NOT EXISTS ${LLVM_DIR}/lib/clang/${LLVM_VERSION}/wasi/libclang_rt.builtins-wasm32.a)
    FetchContent_Declare(
            wasisdk_clang
            URL https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${WASISDK_VERSION}/libclang_rt.builtins-wasm32-wasi-${WASISDK_VERSION}.0.tar.gz
    )

    FetchContent_Populate(wasisdk_clang)

    file(GLOB WASISDK_CLANG_ALL_FILES
            ${wasisdk_clang_SOURCE_DIR}/*)
    file(COPY ${WASISDK_CLANG_ALL_FILES}
            DESTINATION ${LLVM_DIR}/lib/clang/${LLVM_VERSION}/lib/
    )
    message(STATUS "Libclang_rt.builtins-wasm32.as is installed successfully in " ${LLVM_DIR}/lib/clang/${LLVM_VERSION}/wasi/lib)
else()
    message(STATUS "Libclang_rt.builtins-wasm32.as is already installed in " ${LLVM_DIR}/lib/clang/${LLVM_VERSION}/wasi/lib)
endif()
