#include "query_generator.h"

#include <calculator/cube.h>
#include <InA_query_model/Query.h>
#include <InA_query_model/Dimension.h>

#include <sstream>
#include <iostream>

namespace query_generator
{
    std::string query_generator::getSQL() const
    {
        std::string delim;
        std::ostringstream sql;
        std::ostringstream group_by;
        
        sql << "SELECT ";

        for (const auto& dim : m_query.getDefinition().getDimensions())
        {

			const std::string& nameDim = dim.getName();
			if(nameDim == "CustomDimension1")
			{
				for(const auto& member : dim.getMembers() )
				{
					sql << delim;
					sql << member.getAggregation() << "(" << member.getName() << ")";
				}
			}
			else
			{
				sql << delim;
				sql << nameDim;
				group_by << delim << nameDim;
			}

            delim = ", ";
        }

        const std::string& table = m_query.getDataSource().getObjectName();
        sql << " FROM " << table;

        if(!group_by.str().empty())
        {
            sql << " GROUP BY " << group_by.str();
        }

        sql << ";";

        return sql.str();

    }
	
	void query_generator::prepareCube(cube::Cube& cube) const
	{
        for (const auto& dim : m_query.getDefinition().getDimensions())
        {
			const std::string& nameDim = dim.getName();
			if(nameDim == "CustomDimension1")
			{
				for(const auto& member : dim.getMembers() )
					cube.addColumnMeas(dim.getName());
			}
			else
			{
				cube.addColumnDim(dim.getName());
			}
        }
	}
} //query_generator