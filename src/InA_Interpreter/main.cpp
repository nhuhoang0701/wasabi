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

WASM_EXPORT
const char* json_getResponse_json(const char* InA)
{
	std::cout << "c++ json_getResponse_json: received from JS:" << InA << '\n';
	return "{\"hardcoded json returned from c++ json_getResponse_json\":2}";
}
