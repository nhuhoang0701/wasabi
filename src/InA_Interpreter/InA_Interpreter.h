

#define WASM_EXPORT extern "C"

WASM_EXPORT
const char* json_getServerInfo();

WASM_EXPORT
const char* json_getResponse_json(const char* InA);
	