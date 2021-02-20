#include <iostream>
#include "test_data.h"
#include "json/jsonReader.h"

#define WASM_EXPORT extern "C"


WASM_EXPORT
const char* json_getServerInfo()
{
	return "{\"hardcoded json returned from c++ json_getServerInfo\":1}";
}

std::string static_str;
WASM_EXPORT
const char* json_getResponse_json(const char* InA)

{
	JSONReader reader;
	JSONGenericObject root = reader.parse(Json_test_reader0);
	std::cout << "c++ json_getResponse_json: received from JS:" << InA << '\n';
	static_str += "{";
	static_str += "\"c++ received hardcoded json received from JS json_getResponse_json\":\"";
	static_str += InA;
	static_str += "\"";
	static_str += "}";
	
	return static_str.c_str();
}
