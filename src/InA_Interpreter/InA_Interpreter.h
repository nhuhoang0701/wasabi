
#include "json/jsonReader.h"
#include "json/jsonWriter.h"

#define WASM_EXPORT extern "C"

WASM_EXPORT
const char* json_getServerInfo();

WASM_EXPORT
const char* json_getResponse_json(const char* InA);

// Internals
namespace ina_interpreter
{
	const char* processRequest(const JSONGenericObject& object, JSONWriter& writer);
	void processAnalyticsRequest(const JSONGenericObject& object, JSONWriter& writer);
}
	