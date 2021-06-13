#include "InA_Interpreter.h"
#include "calculator/storage.h"

#include <iostream>

#include <stdexcept>
#include <string>
#include <test_tools/TestAssert.h>

#include <InA_query_model/Dimension.h>

static int32_t ID = 0;
extern "C"
void ina_callback_response(int ID, const char* action, const char* response)
{
    ScopeLog sc("ina_callback_response");
    CPPUNIT_ASSERT(response!=nullptr);
    if(ID == 0)
    {
        CPPUNIT_ASSERT_EQUAL_STR("GetServerInfo", action);
        CPPUNIT_ASSERT(!std::string(response).empty());
        // std::cout << "InA_Interpreter_test => response: " << std::string(response).substr(0, 120) << " ..." << std::endl;
    }
    else if(ID == 1)
    {
        CPPUNIT_ASSERT_EQUAL_STR("GetResponse", action);
        CPPUNIT_ASSERT(!std::string(response).empty());
        // std::cout << "InA_Interpreter_test => response: " << response << std::endl;
    }
    else if(ID == 2)
    {
        CPPUNIT_ASSERT_EQUAL_STR("GetResponse", action);
        CPPUNIT_ASSERT(!std::string(response).empty());
        // std::cout << "InA_Interpreter_test => response: " << response << std::endl;
    }
    else
    {
        throw std::runtime_error("Unkow message ID:" + std::to_string(ID));
    }
}

void getServerInfo();
void getResponse();
void interpreter();

int main()
{
    ScopeLog sc("InA_Interpreter_test");
    TEST_INIT();

    getServerInfo();

    getResponse();

    return TEST_HAVEERROR();
}

void getServerInfo()
{
    ScopeLog sc("getServerInfo");
    void_getServerInfo_int32(ID++);
}

void getResponse()
{
    ScopeLog sc("getResponse");
	std::string request;

    request = R"({"Metadata":{"DataSource": {"ObjectName": "onetable_datatype","PackageName":"local:sqlite:onetable_datatype"}, "Expand":["Cube"]}})";
    void_getResponse_int32_json(ID++, request.c_str());

    request = R"({"Analytics":{"DataSource":{"ObjectName":"onetable_datatype","PackageName":"local:sqlite:onetable_datatype","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"text","Axis":"Rows","Attributes":[{"Name":"text"}]},{"Name":"varchar","Axis":"Rows","Attributes":[{"Name":"varchar"}]},{"Name":"CustomDimension1","Axis":"Columns","Attributes":[{"Name":"measure"}],"Members":[{"Description":"Measure 1","Name":"real", "Aggregation":"SUM"}]}]}}})";
    void_getResponse_int32_json(ID++, request.c_str());
}
