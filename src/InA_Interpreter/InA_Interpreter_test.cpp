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

    request = R"({"Metadata":{"DataSource": {"ObjectName": "$$DataSource$$","PackageName":"local:sqlite:onetable_datatype"}, "Expand":["Cube"]}})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;
    std::cout << "--------------------- getResponse ---------------------" << std::endl;

    request = R"({"Metadata":{"DataSource": {"ObjectName": "onetable_datatype","PackageName":"local:sqlite:onetable_datatype"}, "Expand":["Cube"]}})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;

    request = R"({"Batch": [{"Metadata":{"DataSource": {"ObjectName": "$$DataSource$$","PackageName":"local:sqlite:onetable_datatype"}, "Expand":["Cube"]}},{"Analytics": {}},{"Analytics": {}}]})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;

    request = R"({"Analytics":{"DataSource":{"ObjectName":"onetable_datatype","PackageName":"local:sqlite:onetable_datatype","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"text","Axis":"Rows"},{"Name":"varchar","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"real", "Aggregation":"SUM"}]}]}}})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;
}
