#include "ModelSQLGenerator.h"
#include <string>

namespace query_generator
{
    std::string generateSQL(const ina::query_model::Element& filter)
    {
        switch (filter.getComparisonOperator()) {
            case ina::query_model::Element::ComparisonOperator::IsNull:
            {
                return generateUnaryOperatorSQL(filter);
            }
            default:
            {
                return generateNAryOperatorSQL(filter);
            }        
        }
    }
    
    std::string generateNAryOperatorSQL(const ina::query_model::Element& filter)
    {
        std::string filterSQL;
        if (filter.isExcluding())
        {
            filterSQL += "NOT ( ";
        }
        // fieldName is never empty 
        filterSQL += filter.getFieldName();
        filterSQL += " " ;
        // comparisonOperator is never empty 
        filterSQL += ina::query_model::Element::toSql(filter.getComparisonOperator());
        // In case of unary operator, now low/high value
        if (!filter.getLowValue().empty())
        {
            filterSQL += " '";
            filterSQL += filter.getLowValue();
            filterSQL += "'";
        }
        if (filter.isExcluding())
        {
            filterSQL += " )";
        }

        return filterSQL;
    }

    std::string generateUnaryOperatorSQL(const ina::query_model::Element& filter)
    {
        std::string filterSQL;
        if (filter.isExcluding())
        {
            filterSQL += "NOT ( ";
        }
        // fieldName is never empty 
        filterSQL += filter.getFieldName();
        filterSQL += " " ;
        // comparisonOperator is never empty 
        filterSQL += ina::query_model::Element::toSql(filter.getComparisonOperator());
        // In case of unary operator, now low/high value
        if (filter.isExcluding())
        {
            filterSQL += " )";
        }

        return filterSQL;
    }

    std::string generateSQL(const ina::query_model::Sort& querySort)
    {
        std::string sortSQL = querySort.getObjectName();
        
        if (querySort.getDirection() == ina::query_model::Sort::Direction::Ascending) 
        {
            sortSQL += " ASC";
        }
        else if (querySort.getDirection() == ina::query_model::Sort::Direction::Descending) 
        {
            sortSQL += " DESC";
        }

        return sortSQL;
    }
}