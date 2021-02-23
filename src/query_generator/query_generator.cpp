#include "query_generator.h"

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
	        //std::cout << "query_generator::getSQL object= " << std::get<0>(object) << " " << std::get<1>(object) << " " << std::get<2>(object) << std::endl;
            query_model::Aggregation agg = std::get<2>(object);
            if(agg.empty())
            {
                sql << delim << std::get<0>(object);
            }
            else
            {
                sql << delim << agg << "(" << std::get<0>(object) << ")";
            }
            delim = ", ";
        }

        sql << " FROM " << table << ";";

        return sql.str();

    }
} //query_generator