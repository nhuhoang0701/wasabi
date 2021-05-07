#pragma once

#include <vector>

class JSONWriter;
namespace ina::query_model { class Query; class Definition; class Dimension; };
namespace calculator {enum class eAxe; };
namespace calculator { class Cube; class Axe; };

namespace ina::grid
{
	void   writeGrid(const ina::query_model::Query& query, JSONWriter& writer);
	void   writeDimensions(const ina::query_model::Query& query, JSONWriter& writer, const calculator::Cube& cube, std::vector<const ina::query_model::Dimension*>& dims);
	size_t writeTuples(const ina::query_model::Query& query, JSONWriter& writer, const calculator::Cube& cube, const calculator::eAxe& axis, std::vector<const ina::query_model::Dimension*>& dims);
	
	std::pair<size_t, size_t>   writeCells(const ina::query_model::Query& query, JSONWriter& writer, const calculator::Cube& cube, const ina::query_model::Dimension* measDim);

	class Grid
	{
	public:
		Grid();
	private:
	};
} // grid
