#pragma once

#include "axis.h"

#include <string>

class JSONWriter;

#include <InA_query_model/Member.h>
namespace ina::query_model { class Query; class Dimension;};
namespace ina::metadata { class Cube; class Dimension;};
namespace calculator {class Cube; class Axe;}

namespace ina::grid
{
	class Grid;
	void   write(const Grid& grid, JSONWriter& writer);

	class Grid
	{
	public:
		Grid(const ina::query_model::Query& query, const ina::metadata::Cube& dsCube, const calculator::Cube& cube);

		const ina::query_model::Query& getQuery() const {return m_query;};
		const calculator::Cube&        getCube() const {return m_cube;};

		const Axis&        getRowAxis() const {return m_rowAxe;};
		const Axis&        getColAxis() const {return m_colAxe;};

		// Row first / Col second
		std::pair<size_t, size_t>      getCellsSize() const;

		size_t   getCellsColumnFrom() const;
		size_t   getCellsColumnTo() const;
		size_t   getCellsRowFrom() const;
		size_t   getCellsRowTo() const;

	private:
		const ina::query_model::Query& m_query;
		const calculator::Cube&        m_cube;

		std::pair<size_t, size_t>      m_cellsSize;

		Axis  m_rowAxe;
		Axis  m_colAxe;

		friend void   write(const Grid& grid, JSONWriter& writer);
	};
} // grid
