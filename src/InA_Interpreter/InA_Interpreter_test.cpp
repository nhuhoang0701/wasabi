#include "InA_Interpreter.h"

#include <iostream>

#include <test_tools/TestAssert.h>

#include <InA_query_model/Dimension.h>

#define WASM_EXPORT extern "C"

void getServerInfo();
void getResponse();
void getCatalogRequestResponse();
int main()
{
    TEST_INIT();

    getServerInfo();

    getResponse();

    getCatalogRequestResponse();

    return TEST_HAVEERROR();
}

void getServerInfo()
{
    std::cout << "-------------------- getServerInfo --------------------" << std::endl;

    const char* response;

    // std::cout << "InA_Interpreter_test => request json_getServerInfo" << std::endl;

    response = json_getServerInfo();

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << std::string(response).substr(0, 120) << " ..." << std::endl;

    std::cout << "-------------------------------------------------------" << std::endl << std::endl;
}

void getResponse()
{
    const char* response = nullptr;
    std::string request;

    std::cout << "--------------------- getResponse ---------------------" << std::endl;

    request = R"({"Metadata":{"DataSource": {"ObjectName": "$$DataSource$$","PackageName":"local:sqlite:onetable_datatype.db"}, "Expand":["Cubes"]}})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;
    std::cout << "--------------------- getResponse ---------------------" << std::endl;

    request = R"({"Metadata":{"DataSource": {"ObjectName": "onetable_datatype","PackageName":"local:sqlite:onetable_datatype.db"}, "Expand":["Cubes"]}})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;

    request = R"({"Batch": [{"Metadata":{"DataSource": {"ObjectName": "$$DataSource$$","PackageName":"local:sqlite:onetable_datatype.db"}, "Expand":["Cubes"]}},{"Analytics": {}},{"Analytics": {}}]})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;

    request = R"({"Analytics":{"DataSource":{"ObjectName":"onetable_datatype","PackageName":"local:sqlite:onetable_datatype.db","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"text","Axis":"Rows"},{"Name":"varchar","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"real", "Aggregation":"SUM"}]}]}}})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;
}
void getCatalogRequestResponse()
{
    std::string request = R"({"Metadata":{"Context":"Analytics","Language":"EN","DataSource":{"Type":"CatalogView","ObjectName":"$$DataSource$$","InstanceId":"073e26bf-643c-1afe-fc55-16331cb7c6af"},"Capabilities":["AggregationNOPNULL","AggregationNOPNULLZERO","AttributeHierarchy","AttributeValueLookup","AverageCountIgnoreNullZero","CalculatedDimension","ClientCapabilities","Conditions","DatasourceAtService","DimensionValuehelpProperty","ExceptionAggregationDimsAndFormulas","ExceptionSettings","Exceptions","ExtendedDimensions","ExtendedDimensionsFieldMapping","ExtendedDimensionsJoinColumns","ExtendedDimensionsOuterJoin","HierarchyKeyTextName","HierarchyLevelOffsetFilter","HierarchyPath","HierarchyPathUniqueName","MaxResultRecords","MetadataCubeQuery","MetadataDataCategory","MetadataHierarchyLevels","MetadataHierarchyStructure","MetadataIsDisplayAttribute","Obtainability","OrderBy","ResultSetAxisType","ResultSetInterval","ResultSetState","ReturnedDataSelection","SupportsCalculatedKeyFigures","SupportsComplexFilters","SupportsEncodedResultSet","SupportsEncodedResultSet2","SupportsExtendedSort","SupportsIgnoreExternalDimensions","SupportsMemberValueExceptions","SupportsMemberVisibility","SupportsRestrictedKeyFigures","SupportsSetOperand","TechnicalAxis","Totals","TotalsAfterVisibilityFilter","Variables","VisibilityFilter"],"Expand":["Cube"]}})";
    std::cout << "--------------------- getCatalogRequestResponse ---------------------" << std::endl;
    std::cout << json_getResponse_json(request.c_str());
    const auto resp = std::string(json_getResponse_json(request.c_str()));
    CPPUNIT_ASSERT(!resp.empty());
    JSONReader reader;
    auto root = reader.parse(resp);
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), root.getArray("message").size());
}
