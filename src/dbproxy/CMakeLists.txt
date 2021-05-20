cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(dbproxy VERSION 1.0)

################################################
# dbproxy
add_library(dbproxy dbproxy.cpp sqlite/sqlite.cpp)
set_target_properties(dbproxy PROPERTIES PUBLIC_HEADER dbproxy.h)
target_include_directories(dbproxy PUBLIC ${SQLITE_INCLUDE_DIRS})

target_link_libraries(dbproxy PUBLIC sqlite exceptions)
target_link_directories(dbproxy PUBLIC ${WASABI_LIBS} ${SQLITE_LIBRARY_DIRS})

################################################
# test
add_executable(dbproxy_test dbproxy_test.cpp)
target_link_libraries(dbproxy_test dbproxy)
target_include_directories(dbproxy_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

add_executable(sqlite_test sqlite/sqlite_test.cpp)
target_link_libraries(sqlite_test dbproxy)
target_include_directories(sqlite_test PRIVATE ${SQLITE_INCLUDE_DIRS} ${WASABI_ROOT_DIR}/src/test_tools/include)

add_executable(dbproxy_metadata_test dbproxy_metadata_test.cpp)
target_link_libraries(dbproxy_metadata_test dbproxy)
target_include_directories(dbproxy_metadata_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

## [main]
add_test(NAME sqlite_test COMMAND sqlite_test --dir=./ WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
add_test(NAME dbproxy_test COMMAND dbproxy_test --dir=./ WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
add_test(NAME dbproxy_metadata_test COMMAND dbproxy_metadata_test --dir=./ WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})

## [installation]
install(TARGETS dbproxy
		LIBRARY DESTINATION ${WASABI_LIBS}
		PUBLIC_HEADER DESTINATION ${WASABI_INCLUDES})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/sqlite/efashion_lite/efashion_lite.db DESTINATION ./resources/sqlite/efashion_lite)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/sqlite/chinook/chinook.db DESTINATION ./resources/sqlite/chinook)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/sqlite/onetable_datatype/onetable_datatype.db DESTINATION ./resources/sqlite/onetable_datatype)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
