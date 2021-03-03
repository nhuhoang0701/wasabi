
#include "json/jsonReader.h"
#include "json/jsonWriter.h"

#define WASM_EXPORT extern "C"

WASM_EXPORT
const char* json_getServerInfo();

WASM_EXPORT
const char* json_getResponse_json(const char* InA);

namespace query_model
{
	class InA_query_model;
};

// Internals
namespace ina_interpreter
{
	const char* processRequest(const JSONGenericObject& object, JSONWriter& writer);

	void        parseAnalyticsRequest(const JSONGenericObject& object, query_model::InA_query_model& queryModel);

	const char* processMetadataRequest(const JSONGenericObject& object);
}
