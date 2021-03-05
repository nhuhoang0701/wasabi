#include "InA_Interpreter.h"

#include <iostream>

#include "test_tools/TestAssert.h"

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

    request = R"({"Metadata":{"DataSource": {"ObjectName": "$$DataSource$$"}, "Expand":["Cubes"]}})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;
    std::cout << "--------------------- getResponse ---------------------" << std::endl;

    request = R"({"Metadata":{"DataSource": {"ObjectName": "Article_Color_Lookup"}, "Expand":["Cubes"]}})";

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

    request = R"({"Batch": [{"Metadata":{"DataSource": {"ObjectName": "$$DataSource$$"}, "Expand":["Cubes"]}},{"Analytics": {}},{"Analytics": {}}]})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;

    request = R"({"Analytics":{"DataSource":{"ObjectName":"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma","PackageName":"local:sqlite:efashion.db","Type":"sqlite"},"Definition":{"Description":"Order count",
            "Dimensions":[{"Name":"Yr","Axis":"Rows"},{"Name":"Qtr","Axis":"Rows"},{"Name":"Sales_revenue","Axis":"Rows"}],
            "DataSource":{"PackageName":"liquid-sqe","ObjectName":"LIQUID_SALES_AV1"},"Name":"Query35",
            "ReadMode":"BookedAndSpaceAndState"}},"Description":"Order count","Name":"QMDS0035",
            "TestNumber":66})";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());
    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "-------------------------------------------------------" << std::endl << std::endl;
}
