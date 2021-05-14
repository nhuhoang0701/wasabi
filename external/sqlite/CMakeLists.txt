cmake_minimum_required(VERSION 2.8.12)

project(sqlite)

message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode")
message(STATUS "Building with ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION} on ${CMAKE_SYSTEM}")

set(CMAKE_C_FLAGS "$ENV{CFLAGS} -Wno-sign-compare -Wno-missing-field-initializers -Wno-int-conversion -Wno-unused-parameter -Wno-implicit-function-declaration")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -DDEBUG -DSQLITE_DEBUG -O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DNDEBUG -O3")


add_definitions(
    -DNDEBUG
    -DSQLITE_OS_OTHER
)

add_library(sqlite
    src/vfs.c
    src/os.c
    src/sqlite3.c
)

if (NOT "${WASABI_PLATFORM_TARGET}" STREQUAL "wasm")
add_executable(sqlite.exe src/shell.c)
target_link_libraries(sqlite.exe PUBLIC sqlite)
endif()

install(FILES src/sqlite3.h DESTINATION ./include)
