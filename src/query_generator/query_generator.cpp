#include "query_generator.h"


#include "../cube/cube.h"

#include <sstream>
#include <iostream>

namespace query_generator
{
    std::string query_generator::getSQL() const
    {
        const std::string& table = m_qryModel.getTable();
        const std::vector<query_model::Object>& objects = m_qryModel.getObjects();

        std::string delim;
        std::ostringstream sql;
        
        sql << "SELECT ";

        for (const auto& object : objects)
        {
            sql << delim;
            
            const query_model::Aggregation& agg = std::get<2>(object);
            const std::string& name = std::get<0>(object);
            // const query_model::Datatype& datatype = std::get<1>(object);
	        // std::cout << "query_generator::getSQL object= " << name << " " << datatype << " " << agg << std::endl;
            if(agg.empty())
            {
                sql << name;
            }
            else
            {
                sql << agg << "(" << name << ")";
            }
            delim = ", ";
        }

        sql << " FROM " << table << ";";

        return sql.str();

    }
	
	void query_generator::prepareCube(cube::Cube& cube)
	{
        const std::vector<query_model::Object>& objects = m_qryModel.getObjects();
        for (const auto& object : objects)
		{
            const query_model::Aggregation& agg = std::get<2>(object);
            const std::string& name = std::get<0>(object);
			if(agg.empty())
				cube.addColumnDim(name);
			else
				cube.addColumnMeas(name);
		}
	}
} //query_generator