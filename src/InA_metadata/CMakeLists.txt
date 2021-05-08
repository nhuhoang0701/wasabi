cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(InA_metadata VERSION 1.0)

################################################
# InA_metadata
add_library(InA_metadata Member.cpp MemberWriter.cpp Attribute.cpp AttributeWriter.cpp Dimension.cpp DimensionWriter.cpp CubeWriter.cpp)
set_target_properties(InA_metadata PROPERTIES PUBLIC_HEADER Attribute.h Dimension.h Member.h)

target_link_libraries(InA_metadata PUBLIC exceptions)
target_link_directories(InA_metadata PUBLIC ${WASABI_LIBS})

################################################
# test
#add_executable(InA_metadata_test grid_test.cpp)
#target_link_libraries(InA_metadata_test InA_metadata)
#target_include_directories(InA_metadata_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)

## [main]
#add_test(NAME InA_metadata COMMAND InA_metadata_test)

## [installation]
install(TARGETS InA_metadata 
		LIBRARY DESTINATION ${WASABI_LIBS}
		PUBLIC_HEADER DESTINATION ${WASABI_INCLUDES})

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
