#include "cells.h"

#include "grid.h"

#include <InA_query_model/Query.h>
#include <InA_query_model/QueryEx.h>
#include <calculator/cube.h>
#include <stdint.h>

namespace ina::grid
{
    Cells::Cells(const Axis& rowAXis, const Axis& colAxis, const calculator::Body& body)
    : m_body(body), m_rowAxis(rowAXis), m_colAxis(colAxis)
    {
    }

    void Cells::init(const Grid& grid)
    {
        m_RowCount = grid.getCube().getBody().getRowCount();
        if( grid.getRowAxis().getMeasureDimension() != nullptr)
        {
            m_RowCount *= grid.getRowAxis().getMeasureDimMembers().size();
            if(getCubeBody().size() < getRowAxis().getMeasureDimMembers().size())
                throw std::runtime_error("Invalid state: Body size is less than visble members size on row.");
        }

        m_ColumnCount = grid.getCube().getBody().getColumnCount();
        if( grid.getColumnAxis().getMeasureDimension() != nullptr)
        {
            m_ColumnCount *= grid.getColumnAxis().getMeasureDimMembers().size();
            if(getCubeBody().size() < getColumnAxis().getMeasureDimMembers().size())
                throw std::runtime_error("Invalid state: Body size is less than visble members size on col.");
        }


        const ina::query_model::SubSetDescription& subsetDescription = grid.getQuery().getQueryDefinition().getResultSetFeat().getSubSetDescription();
        int32_t val = subsetDescription.m_ColumnFrom;
        if(val<0)
            m_ColumnFrom = 0;
        else
            m_ColumnFrom = std::min<size_t>(val, m_ColumnCount);

        val = subsetDescription.m_ColumnTo;
        if(val<0)
            m_ColumnTo = m_ColumnCount;
        else
            m_ColumnTo = std::min<size_t>(val, m_ColumnCount);

        val = subsetDescription.m_RowFrom;
        if(val<0)
            m_RowFrom = 0;
        else
            m_RowFrom = std::min<size_t>(val, m_RowCount);

        val = subsetDescription.m_RowTo;
        if(val<0)
            m_RowTo = m_RowCount;
        else
            m_RowTo = std::min<size_t>(val, m_RowCount);
    }

    const Axis& Cells::getRowAxis() const
    {
        return m_rowAxis;
    }
    const Axis&Cells::getColumnAxis() const
    {
        return m_colAxis;
    }
	const calculator::Body& Cells::getCubeBody() const
    {
        return m_body;
    }

    size_t Cells::getTotalRowCount() const
    {
        return m_RowCount;
    }
    size_t Cells::getTotalColumnCount() const
    {
        return m_ColumnCount;
    }

    size_t Cells::getRowCount() const
    {
        return m_RowTo-m_RowFrom;
    }
    size_t Cells::getColumnCount() const
    {
        return m_ColumnTo-m_ColumnFrom;
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
