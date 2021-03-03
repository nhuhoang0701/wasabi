#include "query_generator.h"

#include <cube/cube.h>
#include <InA_query_model/InA_query_model.h>
#include <InA_query_model/InA_dimension.h>
#include <sstream>
#include <iostream>

namespace query_generator
{
    std::string query_generator::getSQL() const
    {
        const std::string& table = m_qryModel.getTable();
        const std::vector<query_model::InA_dimension>& objects = m_qryModel.getObjects();

        std::string delim;
        std::ostringstream sql;
        std::ostringstream group_by;
        
        sql << "SELECT ";

        for (const auto& object : objects)
        {
            sql << delim;
            
             switch(object.getType())
			{
				case query_model::InA_dimension::Type::ObjectsDimension:
				case query_model::InA_dimension::Type::VariableDimension:
				{
					sql << object.getName();
                    group_by << delim << object.getName();
				}
				break;
				case query_model::InA_dimension::Type::MeasuresDimension:
				{
					sql << "SUM" << "(" << object.getName() << ")";
				}
				break;
				default:
				break;
			}
            delim = ", ";
        }

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
        const std::vector<query_model::InA_dimension>& objects = m_qryModel.getObjects();
        for (const auto& object : objects)
		{
			switch(object.getType())
			{
				case query_model::InA_dimension::Type::ObjectsDimension:
				case query_model::InA_dimension::Type::VariableDimension:
				{
					cube.addColumnDim(object.getName());
				}
				break;
				case query_model::InA_dimension::Type::MeasuresDimension:
				{
					cube.addColumnMeas(object.getName());
				}
				break;
				default:
				break;
			}
		}
	}
} //query_generator