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

//TODO: move the following code  to Grid
class JSONWriter;
namespace ina::query_model { class Definition; class Dimension; };
namespace calculator {enum class eAxe; };
namespace calculator { class Cube; class Axe; };

namespace ina::grid
{
	const char* writeCube(const ina::query_model::Query& query, JSONWriter& writer);

	void   writeGrid(const ina::query_model::Query& query, JSONWriter& writer);
	void   writeDimensions(JSONWriter& writer, std::vector<const ina::query_model::Dimension*>& dims, const calculator::Cube& cube);
	size_t writeTuples(JSONWriter& writer, const query_model::Definition & definition , const calculator::Cube& cube, calculator::eAxe eAxe);
	void   writeTupleValues(JSONWriter &writer, const calculator::Axe& axis, const ina::query_model::Dimension& dimension) ;
	
	std::pair<size_t, size_t>   writeCells(JSONWriter& writer, const calculator::Cube& cube, const ina::query_model::Dimension* measDim);
}
