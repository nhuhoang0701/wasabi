#include "grid.h"
#include "InA_query_model/ResultSetFeatureRequest.h"


#include <InA_query_model/Query.h>
#include <InA_query_model/QueryEx.h>
#include <InA_query_model/Dimension.h>

#include <InA_metadata/Cube.h>

#include <calculator/cube.h>

#include <iostream>
#include <stdexcept>

namespace ina::grid
{
    Grid::Grid(const ina::query_model::QueryEx& queryExec, std::shared_ptr<const calculator::Cube> cube)
    : m_queryExec(queryExec), m_cube(cube),
      m_rowAxe("ROWS", cube->getAxe(calculator::eAxe::Row)), 
      m_colAxe("COLUMNS", cube->getAxe(calculator::eAxe::Column)),
      m_cells(m_rowAxe, m_colAxe, cube->getBody())
    {
        for (const auto& dimension : m_queryExec.getQueryDefinition().getDimensions()) 
        {
            if(dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
                m_rowAxe.addDimension(dimension, m_queryExec);
            else if(dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
                m_colAxe.addDimension(dimension, m_queryExec);
        }

        const ina::query_model::SubSetDescription& subsetDescription = m_queryExec.getQueryDefinition().getResultSetFeat().getSubSetDescription();
        m_rowAxe.init(m_queryExec, subsetDescription.m_RowFrom, subsetDescription.m_RowTo);
        m_colAxe.init(m_queryExec, subsetDescription.m_ColumnFrom, subsetDescription.m_ColumnTo);
        m_cells.init(*this);

        if(!m_rowAxe.getMeasureDimMembers().empty() && m_cells.getTotalRowCount() != m_rowAxe.getTupleTotalCount() )
        {
            throw std::runtime_error("Cells and row Axis have different size");
        }
        if(!m_colAxe.getMeasureDimMembers().empty() &&  m_cells.getTotalColumnCount() !=  m_colAxe.getTupleTotalCount())
        {
            throw std::runtime_error("Cells and col Axis have differnet size");
        }
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