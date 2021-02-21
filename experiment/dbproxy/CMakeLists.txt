cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

set(CMAKE_CXX_FLAGS "-Xlinker --import-memory \
-Xlinker --no-entry \
-Xlinker --export=Sum \
-Xlinker --export=Hello \
-fvisibility=hidden \
-Xlinker --allow-undefined-file=../js_fct.syms")

project(
  Wasabi_ValidationCPP_Support
  VERSION 1.0
  LANGUAGES CXX)

include_directories(${WASABI_ROOT_DIR}/src/test_tools/include)
link_directories(${WASABI_ROOT_DIR}/src/build/lib)

# add_library(CPP_libcxx CPP_libcxx.cpp )

add_executable(MyTest test.cpp)


target_link_libraries(MyTest )

## [main]

enable_testing()
add_test(NAME MyTest COMMAND MyTest)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
