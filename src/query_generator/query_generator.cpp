#include "query_generator.h"

namespace query_generator
{
    std::string query_generator::getSQL() const
    {
        const std::string& table = m_qryModel.getTable();
        const std::vector<query_model::Object>& objects = m_qryModel.getObjects();

        std::string sql = "SELECT ";

        for (std::vector<query_model::Object>::const_iterator object = objects.begin(); object != objects.end(); ++object)
        {
            sql += std::get<0>(*object);
            if ( (object + 1) != objects.end() ) {
                sql += ", ";
            }
        }

        sql += " FROM " + table + ";";
        return sql;

    }
} //query_generator