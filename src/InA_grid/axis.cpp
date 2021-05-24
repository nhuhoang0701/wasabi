#include "axis.h"


#include <InA_query_model/QueryEx.h>
#include <InA_query_model/Dimension.h>

#include <InA_metadata/Cube.h>

#include <calculator/cube.h>

namespace ina::grid
{
    Axis::Axis(const std::string& name, const calculator::Axe& cubeAxis)
     : m_name(name), m_cubeAxe(cubeAxis)
     {}

    void Axis::init(const ina::query_model::QueryEx& queryEx, int32_t from, int32_t to)
    {
        m_tupleCount = m_cubeAxe.getCardinality();
        if( m_measDim != nullptr)
        {
            if(m_tupleCount == 0) // Only dim meas on this axe
                m_tupleCount =m_measureDimensionMembers.size();
            else
                m_tupleCount *= m_measureDimensionMembers.size();
        }

        setFromTo(from, to);
    }

	void  Axis::addDimension(const ina::query_model::Dimension& dimension, const ina::query_model::QueryEx& queryEx)
    {
        m_dimensions.push_back(&dimension);
        if(ina::query_model::QueryEx::isDimensionOfMeasures(dimension) )
        {
            m_measDim = &dimension;
            m_measureDimensionMembers = queryEx.getVisibleMembers(*m_measDim);
        }
    }

	void Axis::setFromTo(int32_t from, int32_t to)
    {
        if(from<0)
            m_from = 0;
        else
            m_from = std::min<size_t>(from, m_tupleCount);

        if(to<0)
            m_to = m_tupleCount;
        else
            m_to = std::min<size_t>(to, m_tupleCount);
    }
	size_t Axis::getFrom() const
    {
        return m_from;
    }

	size_t Axis::getTo() const
    {
        return m_to;
    }
}