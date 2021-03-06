#pragma once

#include "axis.h"
#include "cells.h"

#include <string>

class JSONWriter;

#include <InA_query_model/Member.h>
namespace ina::query_model { class QueryEx;};
namespace calculator {class Cube;}

namespace ina::grid
{
	class Grid;
	void   write(const Grid& grid, JSONWriter& writer);

	class Grid
	{
	public:
		Grid(const ina::query_model::QueryEx& queryExec, std::shared_ptr<const calculator::Cube> cube);

		const ina::query_model::QueryEx& getQuery() const {return m_queryExec;};
		const calculator::Cube&        getCube() const {return *m_cube;};

		const Axis&        getRowAxis() const {return m_rowAxe;};
		const Axis&        getColumnAxis() const {return m_colAxe;};

		const Cells&       getCells() const {return m_cells;}

		size_t   getColumnFrom() const;
		size_t   getColumnTo() const;
		size_t   getRowFrom() const;
		size_t   getRowTo() const;

	private:
		const ina::query_model::QueryEx& m_queryExec;
		std::shared_ptr<const calculator::Cube>  m_cube;

		Axis  m_rowAxe;
		Axis  m_colAxe;

		Cells m_cells;

		friend void   write(const Grid& grid, JSONWriter& writer);
	};
} // grid
