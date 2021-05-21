#pragma once

#include <utility>   // For std::pair

namespace ina::query_model { class Query; class Dimension;};
namespace calculator {class Cube; class Axe;}

namespace ina::grid
{
	class Grid;
	class Cells
	{
		public:
		Cells() = default;
		void init(const Grid& grid);
	
		// Row first / Col second
		size_t      getRowCount() const;
		size_t      getColumnCount() const;

		size_t   getColumnFrom() const;
		size_t   getColumnTo() const;
		size_t   getRowFrom() const;
		size_t   getRowTo() const;

	private:
		// Row first/Column second
		size_t    m_RowCount = 0;
		size_t    m_ColumnCount = 0;

		size_t    m_RowFrom = 0;
		size_t    m_RowTo = 0;
		size_t    m_ColumnFrom = 0;
		size_t    m_ColumnTo = 0;
	};
} // grid
