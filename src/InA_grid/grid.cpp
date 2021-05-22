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
      m_colAxe("COLUMNS", cube.getAxe(calculator::eAxe::Column)),
      m_cells(cube.getBody())
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
        m_cells.init(*this);

        if(!m_rowAxe.getMeasureDimMembers().empty() && m_cells.getTotalRowCount() != m_rowAxe.getTupleTotalCount() )
        {
            throw std::runtime_error("Cells and row Axis have different size");
        }
        if(!m_colAxe.getMeasureDimMembers().empty() &&  m_cells.getTotalColumnCount() !=  m_colAxe.getTupleTotalCount())
        {
            throw std::runtime_error("Cells and col Axis have differnet size");
        }

        m_rowAxe.setFromTo( m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_RowFrom,
                            m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_RowTo);
        m_colAxe.setFromTo( m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_ColumnFrom,
                            m_query.getDefinition().getResultSetFeat().getSubSetDescription().m_ColumnTo);

        //TODO: Only measure on column is implemented
        if(getRowAxis().getMeasureDimension() != nullptr)
            throw std::runtime_error("Measure on rows is not yet implemented");
            
        if(getCells().getCubeBody().size() != getColAxis().getMeasureDimMembers().size())
            throw std::runtime_error("Invalid state: Body size and members numbers should be the same");
    }

    size_t Grid::getColumnFrom() const
    {
        return m_cells.getColumnFrom();
    }
    size_t Grid::getColumnTo() const
    {
        return std::max<uint32_t>(m_cells.getColumnTo(), m_colAxe.getTo());
    }
    size_t Grid::getRowFrom() const
    {
        return m_cells.getRowFrom();
    }
    size_t Grid::getRowTo() const
    {
        return std::max<uint32_t>(m_cells.getRowTo(), m_rowAxe.getTo());
    }
}