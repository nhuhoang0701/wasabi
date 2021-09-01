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
endif()