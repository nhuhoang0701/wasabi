#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
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

	class Axis
	{
		public:
		Axis(const std::string& name, const calculator::Axe& cubeAxis);
		void init();

		const std::string& getName() const {return m_name;};
	
		const std::vector<const ina::query_model::Dimension*>& getDimensions() const {return m_dimensions;};
		std::size_t                                            getTupleCount() const {return m_tupleCount;};
		
		const calculator::Axe&  getCubeAxis() const {return m_cubeAxe;};

		const ina::query_model::Dimension*            getMeasureDimension() const {return m_measDim;}
		const std::vector<ina::query_model::Member>& getMeasureDimMembers() const { return m_measureDimensionMembers;}

		private:
		friend Grid;

		std::string                                     m_name;

		std::vector<const ina::query_model::Dimension*> m_dimensions;
		const ina::query_model::Dimension*              m_measDim = nullptr;
		std::vector<ina::query_model::Member>           m_measureDimensionMembers;

		size_t                  m_tupleCount = 0;
		const calculator::Axe&  m_cubeAxe;
	};

	class Grid
	{
	public:
		Grid(const ina::query_model::Query& query, const ina::metadata::Cube& dsCube, const calculator::Cube& cube);

		const ina::query_model::Query& getQuery() const {return m_query;};
		const calculator::Cube&        getCube() const {return m_cube;};

		const std::vector<const ina::query_model::Dimension*>& getRowDims() const;
        const std::vector<const ina::query_model::Dimension*>& getColDims() const;

		// Row first / Col second
		std::pair<size_t, size_t>      getCellsSize() const;

	private:
		const ina::query_model::Query& m_query;
		const calculator::Cube&        m_cube;

		std::pair<size_t, size_t>      m_cellsSize;

		Axis  m_rowAxe;
		Axis  m_colAxe;

		friend void   write(const Grid& grid, JSONWriter& writer);
	};
} // grid
