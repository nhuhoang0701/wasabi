extern "C" const char* json_getServerInfo();
extern "C" const char* json_getResponse_json(const char* InA);


// all after is internal
#include <vector>

class JSONWriter;
namespace ina::query_model { class Query;};

namespace ina_interpreter
{
	const char* processRequest(const ina::query_model::Query& object, JSONWriter& writer);
}

namespace ina::metadata
{
	const char* writeCube(const ina::query_model::Query& query, JSONWriter& writer);
}
