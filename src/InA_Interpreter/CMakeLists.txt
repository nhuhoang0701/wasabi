cmake_minimum_required(VERSION 3.1...3.19)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(InA_Interpreter VERSION 1.0 LANGUAGES CXX)


################################################
# InA_Interpreter

# InA_Interpreter lib
add_library(InA_Interpreter_lib STATIC InA_Interpreter.cpp)
target_include_directories(InA_Interpreter_lib PUBLIC ${WASABI_INCLUDES} ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(InA_Interpreter_lib PUBLIC cjson json exceptions InA_query_model InA_grid InA_metadata query_generator dbproxy metadata_enrichment sqlite calculator)
target_link_directories(InA_Interpreter_lib PUBLIC ${SQLITE_LIBRARY_DIRS} ${cJSON_LIBRARY_DIRS} ${WASABI_LIBS})


# Combined static lib.
#set(C_LIB  ${WASABI_LIBS}/${CMAKE_STATIC_LIBRARY_PREFIX}combined${CMAKE_STATIC_LIBRARY_SUFFIX})

#add_custom_target(combinedLibGenerator DEPENDS "${C_LIB}" )
#add_custom_command(
#    OUTPUT
#        "${C_LIB}"
#    COMMAND ar -q "${C_LIB}" ${WASABI_LIBS}/${CMAKE_STATIC_LIBRARY_PREFIX}InA_Interpreter_lib${CMAKE_STATIC_LIBRARY_SUFFIX} ${WASABI_LIBS}/${CMAKE_STATIC_LIBRARY_PREFIX}calculator${CMAKE_STATIC_LIBRARY_SUFFIX}
#    DEPENDS ${WASABI_LIBS}/${CMAKE_STATIC_LIBRARY_PREFIX}InA_Interpreter_lib${CMAKE_STATIC_LIBRARY_SUFFIX} ${cJSON_LIBRARY_DIRS}/${CMAKE_STATIC_LIBRARY_PREFIX}cjson${CMAKE_STATIC_LIBRARY_SUFFIX} json exceptions InA_query_model InA_grid InA_metadata query_generator dbproxy metadata_enrichment ${SQLITE_LIBRARY_DIRS}/${CMAKE_STATIC_LIBRARY_PREFIX}sqlite${CMAKE_STATIC_LIBRARY_SUFFIX} calculator
#)

#add_library(InA_Interpreter_libcombined STATIC IMPORTED)
#add_dependencies(InA_Interpreter_libcombined combinedLibGenerator)
#set_property(TARGET InA_Interpreter_libcombined PROPERTY IMPORTED_LOCATION ${C_LIB})

# InA_Interpreter executable
if ("${WASABI_PLATFORM_TARGET}" STREQUAL "wasm")
add_executable(InA_Interpreter InA_Interpreter.cpp)
target_link_options(InA_Interpreter PRIVATE "-mexec-model=reactor")
else()
add_library(InA_Interpreter SHARED InA_Interpreter.cpp)
endif()
target_link_libraries(InA_Interpreter InA_Interpreter_lib)
#target_link_libraries(InA_Interpreter InA_Interpreter_libcombined)

# External entries points
if ("${WASABI_PLATFORM_TARGET}" STREQUAL "wasm")
target_link_options( InA_Interpreter PRIVATE LINKER:--export=doIt,--export=malloc,--export=free)
target_link_options( InA_Interpreter PRIVATE LINKER:--allow-undefined-file=${PROJECT_SOURCE_DIR}/js_fct.syms)
endif()

################################################
# test
add_executable(InA_Interpreter_test InA_Interpreter_test.cpp)
target_link_libraries(InA_Interpreter_test InA_Interpreter_lib)
#target_link_libraries(InA_Interpreter_test InA_Interpreter_libcombined)
target_include_directories(InA_Interpreter_test PRIVATE ${WASABI_INCLUDES} ${WASABI_ROOT_DIR}/src/test_tools/include)

add_test(NAME InA_Interpreter_test COMMAND InA_Interpreter_test --dir=./  WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})

## [installation]
install(TARGETS InA_Interpreter DESTINATION .)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/response_getSerververInfo.json DESTINATION ./resources/)

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)

