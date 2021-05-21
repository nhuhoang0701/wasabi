#include "grid.h"


#include <InA_query_model/Query.h>
#include <InA_query_model/Dimension.h>

#include <InA_metadata/Cube.h>

#include <calculator/cube.h>

#include <iostream>
#include <stdexcept>

namespace ina::grid
{
    Grid::Grid(const ina::query_model::Query& query, const ina::metadata::Cube& dsCube, const calculator::Cube& cube)
    : m_query(query), m_cube(cube),
      m_rowAxe("ROWS", cube.getAxe(calculator::eAxe::Row)), 
      m_colAxe("COLUMNS", cube.getAxe(calculator::eAxe::Column))
    {
        for (const auto& dimension : query.getDefinition().getDimensions()) 
        {
            if(dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
                m_rowAxe.addDimension(dimension, getQuery());
            else if(dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
                m_colAxe.addDimension(dimension, getQuery());
        }

        m_rowAxe.init(getQuery());
        m_colAxe.init(getQuery());

        m_cellsSize = std::make_pair(getCube().getBody().getRowCount(), getCube().getBody().getColumnCount());
        if( m_rowAxe.getMeasureDimension() != nullptr)
            m_cellsSize.first *= m_rowAxe.getMeasureDimMembers().size();
        if( m_colAxe.getMeasureDimension() != nullptr)
            m_cellsSize.second *= m_colAxe.getMeasureDimMembers().size();

        if(!m_rowAxe.getMeasureDimMembers().empty() && m_cellsSize.first != m_rowAxe.getTupleCount() )
        {
            throw std::runtime_error("Cells and row Axis have different size");
        }
        if(!m_colAxe.getMeasureDimMembers().empty() &&  m_cellsSize.second !=  m_colAxe.getTupleCount())
        {
            throw std::runtime_error("Cells and col Axis have differnet size");
        }

        m_rowAxe.setFromTo( m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_RowFrom,
                            m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_RowTo);
        m_colAxe.setFromTo( m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_ColumnFrom,
                            m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_ColumnTo);
    }

    std::pair<size_t, size_t> Grid::getCellsSize() const
    {
        return m_cellsSize;
    }

    size_t Grid::getCellsColumnFrom() const
    {
        const int32_t val = m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_ColumnFrom;
        if(val<0)
            return 0;
        return std::min<size_t>(val, getCellsSize().second);
    }
    size_t Grid::getCellsColumnTo() const
    {
        const int32_t val = m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_ColumnTo;
        if(val<0)
            return getCellsSize().second;
        return std::min<size_t>(val, getCellsSize().second);
    }
    size_t Grid::getCellsRowFrom() const
    {
        const int32_t val = m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_RowFrom;
        if(val<0)
            return 0;
        return std::min<size_t>(val, getCellsSize().first);
    }
    size_t Grid::getCellsRowTo() const
    {
        const int32_t val = m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_RowTo;
        if(val<0)
            return getCellsSize().first;
        return std::min<size_t>(val, getCellsSize().first);
    }
}