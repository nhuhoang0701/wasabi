#pragma once

#include <vector>

class JSONWriter;
namespace ina::query_model { class Query; class Dimension; };


#include <calculator/cube.h>

namespace ina::grid
{
	class Grid;
	void   write(const Grid& grid, JSONWriter& writer);

	class Grid
	{
	public:
		Grid(const ina::query_model::Query& query, const calculator::Cube& cube);

		const ina::query_model::Query& getQuery() const {return m_query;};
		const calculator::Cube&        getCube() const {return m_cube;};

		const ina::query_model::Dimension&   getMeasDim() const {return *measDim;}

		const std::vector<const ina::query_model::Dimension*>& getRowDims() const {return rowAxe;};
        const std::vector<const ina::query_model::Dimension*>& getColDims() const {return colAxe;};

	private:
		const ina::query_model::Query& m_query;
		const calculator::Cube& m_cube;

		const ina::query_model::Dimension* measDim = nullptr;
		std::vector<const ina::query_model::Dimension*> rowAxe;
        std::vector<const ina::query_model::Dimension*> colAxe;
	};
} // grid
