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

    CPPUNIT_ASSERT_EQUAL(1, 1);

    return TEST_HAVEERROR();
}

void getServerInfo()
{
    std::cout << "-------------------- getServerInfo --------------------" << std::endl;

    const char* response;

    std::cout << "InA_Interpreter_test => request json_getServerInfo" << std::endl;

    response = json_getServerInfo();

    std::cout << "InA_Interpreter_test => response: " << std::string(response).substr(0, 120) << " ..." << std::endl;

    std::cout << "-------------------------------------------------------" << std::endl << std::endl;
}

void getResponse()
{
    std::cout << "--------------------- getResponse ---------------------" << std::endl;

    const char* response;

    std::string request = "{\"Metadata\":{\"Expand\":[\"Cubes\"]}}";

    std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    std::cout << "InA_Interpreter_test => response: " << response << std::endl;

    std::cout << "------------------------" << std::endl << std::endl;

    request = "{\"Batch\": [{\"Metadata\": {  }},{\"Analytics\": {  }},{\"Analytics\": {  }}]}";

    std::cout << "InA_Interpreter_test => request: json_getResponse_json " << request << std::endl;

    response = json_getResponse_json(request.c_str());

    std::cout << "InA_Interpreter_test => response: " << response << std::endl;


    std::cout << "-------------------------------------------------------" << std::endl << std::endl;
}
