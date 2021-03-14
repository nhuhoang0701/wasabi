
#include <json/jsonReader.h>
#include <json/jsonWriter.h>

#define WASM_EXPORT extern "C"

WASM_EXPORT
const char* json_getServerInfo();

WASM_EXPORT
const char* json_getResponse_json(const char* InA);

namespace ina::query_model { class Query; };

// Internals
namespace ina_interpreter
{
	const char* processRequest(const ina::query_model::Query& object, JSONWriter& writer);
}
