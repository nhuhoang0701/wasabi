#include "grid.h"


#include <InA_query_model/Query.h>
#include <InA_query_model/Dimension.h>

#include <json/jsonWriter.h>

#include <iostream>

namespace ina::grid
{
    Grid::Grid(const ina::query_model::Query& query, const calculator::Cube& cube)
    : m_query(query), m_cube(cube)
    {
        for (const auto& dimension : query.getDefinition().getDimensions()) 
        {
            if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
                rowAxe.push_back(&dimension);
            else if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
                colAxe.push_back(&dimension);

            if(ina::query_model::Dimension::isDimensionOfMeasures(dimension) )
                measDim = &dimension;
        }
    }
}