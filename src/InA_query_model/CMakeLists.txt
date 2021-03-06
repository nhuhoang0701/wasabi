cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED WASABI_ROOT_DIR AND DEFINED ENV{WASABI_ROOT_DIR})
    set (WASABI_ROOT_DIR "$ENV{WASABI_ROOT_DIR}" CACHE PATH "project root" FORCE)
endif()
include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.cmake)

project(InA_query_model VERSION 1.0)
  
################################################
# InA_query_model
add_library(InA_query_model Queries.cpp QueriesReader.cpp ResultSetFeatureRequest.cpp ResultSetFeatureRequestReader.cpp Definition.cpp DefinitionReader.cpp Parameter.cpp Function.cpp ParameterReader.cpp FunctionReader.cpp Formula.cpp FormulaReader.cpp Member.cpp MemberReader.cpp Attribute.cpp AttributeReader.cpp Dimension.cpp DimensionReader.cpp DataSource.cpp DataSourceReader.cpp DataSourceWriter.cpp Query.cpp QueryEx.cpp QueryReader.cpp Sort.cpp SortReader.cpp Selection/Selection.cpp Selection/SelectionReader.cpp Selection/SelectionElement.cpp Selection/SelectionElementReader.cpp Selection/Element.cpp Selection/ElementReader.cpp)
set_target_properties(InA_query_model PROPERTIES PUBLIC_HEADER Query.h)

target_include_directories(InA_query_model PUBLIC ${WASABI_INCLUDES}})
target_link_libraries(InA_query_model PUBLIC cjson json exceptions metadata_enrichment InA_metadata)
target_link_directories(InA_query_model PUBLIC ${cJSON_LIBRARY_DIRS} ${WASABI_LIBS})

################################################
# test
add_executable(InA_query_model_test InA_query_model_test.cpp)
target_include_directories(InA_query_model_test PRIVATE ${WASABI_ROOT_DIR}/src/test_tools/include)
target_link_libraries(InA_query_model_test InA_query_model)
target_link_directories(InA_query_model_test PUBLIC)

## [main]
add_test(NAME InA_query_model_test COMMAND InA_query_model_test)

## [installation]
install(TARGETS InA_query_model 
        LIBRARY DESTINATION ${WASABI_LIBS}
        PUBLIC_HEADER DESTINATION ${WASABI_INCLUDES})

include (${WASABI_ROOT_DIR}/scripts/cmake/wasabi.post.cmake)
