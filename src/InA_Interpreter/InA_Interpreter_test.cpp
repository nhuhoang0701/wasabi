#include "InA_Interpreter.h"

#include <iostream>

#include <test_tools/TestAssert.h>

#include <InA_query_model/Dimension.h>

#define WASM_EXPORT extern "C"

void getServerInfo();
void getResponse();

int main()
{
    TEST_INIT();

    getServerInfo();

    getResponse();

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
    request = R"({"Analytics":{"Definition":{"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"=","Low":"OBJ_147"},{"Comparison":"=","Low":"OBJ_191"}],"FieldName":"[Measures].[Measures]"}}]}}}}}})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());

    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;
    request = R"({"Analytics":{"Definition":{"Dimensions":[{"Members":[{"Description":"Calculated Measure 1","Formula":{"Function":{"Name":"**","Parameters":[{"Member":{"Name":"OBJ_147"}},{"Function":{"Name":"decfloat","Parameters":[{"Constant":{"Value":"2","ValueType":"String"}}]}}]}},"Name":"32160367-6930-4537-9181-755582731239"}],"Axis":"Rows","Name":"CustomDimension1"}]}}})";;

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

    request = R"({"Analytics":{"DataSource":{"ObjectName":"onetable_datatype","PackageName":"local:sqlite:onetable_datatype.db","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"text","Axis":"Rows"},{"Name":"varchar","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Cols","Members":[{"Description":"Measure 1","Name":"real", "Aggregation":"SUM"}]}]}}})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;

	{
		request = R"({"Name":"dimName","Axis":"Rows","Attributes":[{"Name":"ATTR0","Obtainability":"UserInterface"}, {"Name":"ATTR1","Obtainability":"UserInterface"}]})";
		JSONReader reader;
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Dimension dimension;
		read(dimension, root);

		CPPUNIT_ASSERT_EQUAL(2, dimension.getAttributes().size());
		CPPUNIT_ASSERT_EQUAL_STR("ATTR0", dimension.getAttributes().at(0).getName().c_str());
		CPPUNIT_ASSERT_EQUAL_STR("ATTR1", dimension.getAttributes().at(1).getName().c_str());
    }

    std::cout << "-------------------------------------------------------" << std::endl << std::endl;
}
