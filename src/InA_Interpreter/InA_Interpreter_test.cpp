#include "InA_Interpreter.h"
#include "calculator/storage.h"

#include <iostream>

#include <test_tools/TestAssert.h>

#include <InA_query_model/Dimension.h>

void getServerInfo();
void getResponse();
void interpreter();

int main()
{
    TEST_INIT();

    getServerInfo();

    getResponse();

    interpreter();

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

    request = R"({"Metadata":{"DataSource": {"ObjectName": "onetable_datatype","PackageName":"local:sqlite:onetable_datatype"}, "Expand":["Cube"]}})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());

    std::cout << "------------------------" << std::endl << std::endl;

    request = R"({"Analytics":{"DataSource":{"ObjectName":"onetable_datatype","PackageName":"local:sqlite:onetable_datatype","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"text","Axis":"Rows"},{"Name":"varchar","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"real", "Aggregation":"SUM"}]}]}}})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;
}

#include <InA_query_model/Formula.h>
#include <InA_query_model/Parameter.h>
#include <json/jsonReader.h>
void interpreter()
{
	JSONReader reader;

    ina::query_model::Function fct;
    read(fct, reader.parse(R"({"Name": "+","Parameters": [{"Constant": {"ValueType": "String","Value": "1"}},{"Function": {"Name": "*","Parameters": [{"Constant": {"ValueType": "String","Value": "2"}},{"Function": {"Name": "/","Parameters": [{"Function": {"Name": "+","Parameters": [{"Constant": {"ValueType": "String","Value": "3"}},{"Constant": {"ValueType": "String","Value": "4"}}]}},{"Constant": {"ValueType": "String","Value": "5"}}]}}]}}]})"));

    CPPUNIT_ASSERT_EQUAL(3.8, std::get<double>(evalFunction(fct)));
}
