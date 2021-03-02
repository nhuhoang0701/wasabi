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
    std::cout << "--------------------- getResponse ---------------------" << std::endl;

    const char* response;

    std::string request = "{\"Metadata\":{\"Expand\":[\"Cubes\"]}}";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());

    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;

    request = "{\"Batch\": [{\"Metadata\": {  }},{\"Analytics\": {}},{\"Analytics\": {}}]}";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());

    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;

    request = "{\"Analytics\":{\"DataSource\":{\"CustomProperties\":{\"cnxString\":\"local:sqlite:efashion.db\"},"
            "\"ObjectName\":\"Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma\",\"PackageName\":\"/\",\"Type\":\"sqlite\"},\"Definition\":{\"Description\":\"Order count\","
            "\"Dimensions\":[{\"Name\":\"Yr\",\"Axis\":\"Rows\"},{\"Name\":\"Qtr\",\"Axis\":\"Rows\"},{\"Name\":\"Sales_revenue\",\"Axis\":\"Rows\"}],"
            "\"DataSource\":{\"PackageName\":\"liquid-sqe\",\"ObjectName\":\"LIQUID_SALES_AV1\"},\"Name\":\"Query35\","
            "\"ReadMode\":\"BookedAndSpaceAndState\"}},\"Description\":\"Order count\",\"Name\":\"QMDS0035\","
            "\"TestNumber\":66}";

    // std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    CPPUNIT_ASSERT(!std::string(response).empty());

    // std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "-------------------------------------------------------" << std::endl << std::endl;
}
