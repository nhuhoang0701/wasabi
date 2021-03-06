#pragma once

#include "calculator/cube.h"
#include <utility>   // For std::pair

namespace ina::query_model { class Query; class Dimension;};
namespace calculator {class Cube; class Axe;}

namespace ina::grid
{
	class Grid;
	class Axis;
	class Cells
	{
		public:
		Cells(const Axis& rowAXis, const Axis& colAxis, const calculator::Body& body);
		void init(const Grid& grid);
		
		const Axis&               getRowAxis() const;
		const Axis&               getColumnAxis() const;
		const calculator::Body&   getCubeBody() const;
	
		size_t      getTotalRowCount() const;
		size_t      getTotalColumnCount() const;

		size_t      getRowCount() const;
		size_t      getColumnCount() const;

		size_t   getColumnFrom() const;
		size_t   getColumnTo() const;
		size_t   getRowFrom() const;
		size_t   getRowTo() const;

	private:
		const calculator::Body&        m_body;
		const Axis&        m_rowAxis;
		const Axis&        m_colAxis;

		// Row first/Column second
		size_t    m_RowCount = 0;
		size_t    m_ColumnCount = 0;

		size_t    m_RowFrom = 0;
		size_t    m_RowTo = 0;
		size_t    m_ColumnFrom = 0;
		size_t    m_ColumnTo = 0;
	};
} // grid
