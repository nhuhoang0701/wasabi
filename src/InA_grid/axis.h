#pragma once

#include <vector>
#include <string>

class JSONWriter;

#include <InA_query_model/Member.h>
namespace ina::query_model { class Query; class Dimension;};
namespace ina::metadata { class Cube; class Dimension;};
namespace calculator {class Cube; class Axe;}

namespace ina::grid
{
	class Axis
	{
		public:
		Axis(const std::string& name, const calculator::Axe& cubeAxis);
		void init(const ina::query_model::Query& query);

		const std::string& getName() const {return m_name;};
	
		const std::vector<const ina::query_model::Dimension*>& getDimensions() const {return m_dimensions;};
		void  addDimension(const ina::query_model::Dimension& dim, const ina::query_model::Query& query);

		std::size_t                                            getTupleCount() const {return m_tupleCount;};
		
		void     setFromTo(int32_t from, int32_t to);
		size_t   getFrom() const;
		size_t   getTo() const;
		
		const calculator::Axe&  getCubeAxis() const {return m_cubeAxe;};

		const ina::query_model::Dimension*            getMeasureDimension() const {return m_measDim;}
		const std::vector<ina::query_model::Member>& getMeasureDimMembers() const { return m_measureDimensionMembers;}

		private:
		std::string                                     m_name;

		std::vector<const ina::query_model::Dimension*> m_dimensions;
		const ina::query_model::Dimension*              m_measDim = nullptr;
		std::vector<ina::query_model::Member>           m_measureDimensionMembers;

		size_t                  m_tupleCount = 0;
		size_t                  m_from = 0;
		size_t                  m_to = 0;
		const calculator::Axe&  m_cubeAxe;
	};
} // grid
