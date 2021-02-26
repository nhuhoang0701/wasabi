cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(
  Wasabi_ValidationC_Support
  VERSION 1.0
  LANGUAGES C)

include_directories(${WASABI_ROOT_DIR}/src/test_tools/include)
link_directories(${WASABI_ROOT_DIR}/src/build/lib)

#add_library(MyLibExample simple_lib.cpp simple_lib.hpp)
#add_library(C_libc_dl C_libc_dl.c)

# Link each target with other targets or add options, etc.

# Adding something we can run - Output name matches target name
add_executable(C_libc C_libc.c)
add_executable(C_read C_read.c)
add_executable(C_write C_write.c)
add_executable(C_nolib C_nolib.c)
#add_executable(C_libc_dl C_libc_dl.c)



# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error.
#target_link_libraries(C_libc PRIVATE )
#target_link_libraries(C_libc_dl PRIVATE )

add_test(NAME C_nolib COMMAND C_nolib)
add_test(NAME C_libc COMMAND C_libc --dir=../resources/  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)
add_test(NAME C_read COMMAND C_read --dir=../resources/  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)
add_test(NAME C_write COMMAND C_write --dir=../resources/  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
