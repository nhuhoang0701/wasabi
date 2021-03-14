#include "query_generator.h"

#include <calculator/cube.h>
#include <InA_query_model/Query.h>
#include <InA_query_model/Dimension.h>

#include <sstream>
#include <iostream>
#include <stdexcept>

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
					cube.addMeas(dim.getName());
			}
			else
			{
				switch (dim.getAxe())
				{
				case ina::query_model::Dimension::eAxe::Rows:
				{
					cube.addDim(cube::Cube::eAxe::Row,dim.getName());
					break;
				}
				case ina::query_model::Dimension::eAxe::Columns:
				{
					cube.addDim(cube::Cube::eAxe::Column,dim.getName());
					break;
				}
				default:
					throw std::runtime_error("Unknow axe");
				}
			}
        }
	}
} //query_generator
