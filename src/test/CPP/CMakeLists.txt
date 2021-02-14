cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(
  Wasabi_ValidationCPP_Support
  VERSION 1.0
  LANGUAGES CXX)

include_directories(${WASABI_ROOT_DIR}/src/test_tools/include)
link_directories(${WASABI_ROOT_DIR}/src/build/lib)

add_library(CPP_libcxx CPP_libcxx.cpp )

add_executable(CPP_nolib_class ../CPP_nolib_class.cpp)
add_executable(CPP_nolib_exception ../CPP_nolib_exception.cpp)
add_executable(CPP_Withlibcxx ../CPP_Withlibcxx.cpp )


target_link_libraries(CPP_nolib_exception PRIVATE exceptions.wasm )
target_link_libraries(CPP_Withlibcxx PRIVATE CPP_libcxx exceptions.wasm )

## [main]
enable_testing()
add_test(NAME CPP_nolib_class COMMAND CPP_nolib_class)
add_test(NAME CPP_nolib_exception_failed COMMAND CPP_nolib_exception)
add_test(NAME CPP_Withlibcxx COMMAND CPP_Withlibcxx)

#Set test as FAILED, exception didn't works in WAsm
set_tests_properties(CPP_nolib_exception_failed PROPERTIES WILL_FAIL TRUE)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
