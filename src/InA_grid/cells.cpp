#include "cells.h"

#include "grid.h"

#include <InA_query_model/Query.h>
#include <calculator/cube.h>
#include <stdint.h>

namespace ina::grid
{
    void Cells::init(const Grid& grid)
    {
        m_RowCount = grid.getCube().getBody().getRowCount();
        if( grid.getRowAxis().getMeasureDimension() != nullptr)
            m_RowCount *= grid.getRowAxis().getMeasureDimMembers().size();

        m_ColumnCount = grid.getCube().getBody().getColumnCount();
        if( grid.getColAxis().getMeasureDimension() != nullptr)
            m_ColumnCount *= grid.getColAxis().getMeasureDimMembers().size();


        int32_t val = grid.getQuery().getDefinition().getResultSetFeat().getSubSetDescription().m_ColumnFrom;
        if(val<0)
            m_ColumnFrom = 0;
        else
            m_ColumnFrom = std::min<size_t>(val, m_ColumnCount);

        val = grid.getQuery().getDefinition().getResultSetFeat().getSubSetDescription().m_ColumnTo;
        if(val<0)
            m_ColumnTo = m_ColumnCount;
        else
            m_ColumnTo = std::min<size_t>(val, m_ColumnCount);

        val = grid.getQuery().getDefinition().getResultSetFeat().getSubSetDescription().m_RowFrom;
        if(val<0)
            m_RowFrom = 0;
        else
            m_RowFrom = std::min<size_t>(val, m_RowCount);

        val = grid.getQuery().getDefinition().getResultSetFeat().getSubSetDescription().m_RowTo;
        if(val<0)
            m_RowTo = m_RowCount;
        else
            m_RowTo = std::min<size_t>(val, m_RowCount);
    }

    size_t Cells::getRowCount() const
    {
        return m_RowCount;
    }
    size_t Cells::getColumnCount() const
    {
        return m_ColumnCount;
    }
    
    size_t Cells::getColumnFrom() const
    {
        return m_ColumnFrom;
    }
    size_t Cells::getColumnTo() const
    {
        return m_ColumnTo;
    }
    size_t Cells::getRowFrom() const
    {
        return m_RowFrom;
    }
    size_t Cells::getRowTo() const
    {
        return m_RowTo;
    }
}
