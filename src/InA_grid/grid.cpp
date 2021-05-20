#include "grid.h"


#include <InA_query_model/Query.h>
#include <InA_query_model/Dimension.h>

#include <InA_metadata/Cube.h>

#include <algorithm>
#include <calculator/cube.h>

#include <cstddef>
#include <json/jsonWriter.h>

#include <iostream>
#include <stdexcept>

namespace ina::grid
{
    Axis::Axis(const std::string& name, const calculator::Axe& cubeAxis)
     : m_name(name), m_cubeAxe(cubeAxis)
     {}

    void Axis::init()
    {
        m_tupleCount = m_cubeAxe.getCardinality();
        if( m_measDim != nullptr)
        {
            if(m_tupleCount == 0) // Only dim meas on this axe
                m_tupleCount =m_measureDimensionMembers.size();
            else
                m_tupleCount *= m_measureDimensionMembers.size();
        }
    }

    Grid::Grid(const ina::query_model::Query& query, const ina::metadata::Cube& dsCube, const calculator::Cube& cube)
    : m_query(query), m_cube(cube),
      m_rowAxe("ROWS", cube.getAxe(calculator::eAxe::Row)), 
      m_colAxe("COLUMNS", cube.getAxe(calculator::eAxe::Column))
    {
        for (const auto& dimension : query.getDefinition().getDimensions()) 
        {
            Axis* axe = nullptr;
            if(dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
                axe = &m_rowAxe;
            else if(dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
                axe = &m_colAxe;

            axe->m_dimensions.push_back(&dimension);
            if(ina::query_model::Dimension::isDimensionOfMeasures(dimension) )
            {
                axe->m_measDim = &dimension;
                axe->m_measureDimensionMembers = getQuery().getDefinition().getVisibleMembers(*axe->m_measDim);
            }
        }
        m_rowAxe.init();
        m_colAxe.init();

        m_cellsSize = std::make_pair(getCube().getBody().getRowCount(), getCube().getBody().getColumnCount());
        if( m_rowAxe.m_measDim != nullptr)
            m_cellsSize.first *= m_rowAxe.m_measureDimensionMembers.size();
        if( m_colAxe.m_measDim != nullptr)
            m_cellsSize.second *= m_colAxe.m_measureDimensionMembers.size();

        if(m_cellsSize.first != m_rowAxe.m_tupleCount || m_cellsSize.second !=  m_colAxe.m_tupleCount)
            throw std::runtime_error("Cells and Axis have differnet size");
    }

    std::pair<size_t, size_t> Grid::getCellsSize() const
    {
        return m_cellsSize;
    }

    size_t Grid::getColumnFrom() const
    {
        const int32_t val = m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_ColumnFrom;
        if(val<0)
            return 0;
        return std::min<size_t>(val, getCellsSize().second);
    }
    size_t Grid::getColumnTo() const
    {
        const int32_t val = m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_ColumnTo;
        if(val<0)
            return getCellsSize().second;
        return std::min<size_t>(val, getCellsSize().second);
    }
    size_t Grid::getRowFrom() const
    {
        const int32_t val = m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_RowFrom;
        if(val<0)
            return 0;
        return std::min<size_t>(val, getCellsSize().first);
    }
    size_t Grid::getRowTo() const
    {
        const int32_t val = m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_RowTo;
        if(val<0)
            return getCellsSize().first;
        return std::min<size_t>(val, getCellsSize().first);
    }
}