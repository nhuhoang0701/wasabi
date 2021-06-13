#include "InA_Interpreter.h"
#include "calculator/storage.h"

#include <iostream>

#include <stdexcept>
#include <stdint.h>
#include <string>
#include <test_tools/TestAssert.h>

#include <InA_query_model/Dimension.h>

static int32_t CALLBACK_NB_CALL = 0;

static int32_t ID = 0;
extern "C"
void ina_callback_response(int32_t ID, eRequestType type, const char* response)
{
    ScopeLog sc("ina_callback_response");
    CPPUNIT_ASSERT(response!=nullptr);
    CALLBACK_NB_CALL++;
    Logger::log("CALLBACK_NB_CALL", CALLBACK_NB_CALL);
    if(ID == 0)
    {
        WASABI_CHECK_EQUAL(eRequestType::eGetServerInfo, type);
        CPPUNIT_ASSERT(!std::string(response).empty());
        // std::cout << "InA_Interpreter_test => response: " << std::string(response).substr(0, 120) << " ..." << std::endl;
    }
    else if(ID == 1)
    {
        WASABI_CHECK_EQUAL(eRequestType::eGetResponse, type);
        CPPUNIT_ASSERT(!std::string(response).empty());
        // std::cout << "InA_Interpreter_test => response: " << response << std::endl;
    }
    else if(ID == 2)
    {
        WASABI_CHECK_EQUAL(eRequestType::eGetResponse, type);
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
    CALLBACK_NB_CALL = 0;
    doIt(ID++, eRequestType::eGetServerInfo, nullptr);
    WASABI_CHECK_EQUAL(1, CALLBACK_NB_CALL);
}

void getResponse()
{
    ScopeLog sc("getResponse");
	std::string request;

    request = R"({"Metadata":{"DataSource": {"ObjectName": "onetable_datatype","PackageName":"local:sqlite:onetable_datatype"}, "Expand":["Cube"]}})";
    CALLBACK_NB_CALL = 0;
    doIt(ID++, eRequestType::eGetResponse, request.c_str());
    WASABI_CHECK_EQUAL(1, CALLBACK_NB_CALL);

    request = R"({"Analytics":{"DataSource":{"ObjectName":"onetable_datatype","PackageName":"local:sqlite:onetable_datatype","Type":"Wasabi"},"Definition":{"Dimensions":[{"Name":"text","Axis":"Rows","Attributes":[{"Name":"text"}]},{"Name":"varchar","Axis":"Rows","Attributes":[{"Name":"varchar"}]},{"Name":"CustomDimension1","Axis":"Columns","Attributes":[{"Name":"measure"}],"Members":[{"Description":"Measure 1","Name":"real", "Aggregation":"SUM"}]}]}}})";
    CALLBACK_NB_CALL = 0;
    doIt(ID++, eRequestType::eGetResponse, request.c_str());
    WASABI_CHECK_EQUAL(1, CALLBACK_NB_CALL);
}
