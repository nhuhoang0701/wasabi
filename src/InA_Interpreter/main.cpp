#include <iostream>

#define WASM_EXPORT extern "C"

int main()
{
	std::cout << "main executed:"
              << __FILE__ << ':'
              << __LINE__ << '\n';
	return 0;
}

WASM_EXPORT
const char* json_getServerInfo()
{
	return "{\"hardcoded json returned from c++ json_getServerInfo\":1}";
}

std::string static_str;
WASM_EXPORT
const char* json_getResponse_json(const char* InA)
{
	std::cout << "c++ json_getResponse_json: received from JS:" << InA << '\n';
	static_str += "{";
	static_str += "\"c++ received hardcoded json received from JS json_getResponse_json\":\"";
	static_str += InA;
	static_str += "\"";
	static_str += "}";
	
	return static_str.c_str();
}
