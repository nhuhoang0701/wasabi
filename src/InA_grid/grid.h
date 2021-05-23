#pragma once

#include "axis.h"
#include "cells.h"

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
		const Axis&        getColumnAxis() const {return m_colAxe;};

		const Cells&       getCells() const {return m_cells;}

		size_t   getColumnFrom() const;
		size_t   getColumnTo() const;
		size_t   getRowFrom() const;
		size_t   getRowTo() const;

	private:
		const ina::query_model::Query& m_query;
		const calculator::Cube&        m_cube;

		Axis  m_rowAxe;
		Axis  m_colAxe;

		Cells m_cells;

		friend void   write(const Grid& grid, JSONWriter& writer);
	};
} // grid
