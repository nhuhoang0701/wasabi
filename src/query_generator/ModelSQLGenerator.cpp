#include "InA_query_model/QueryFilter.h"
#include "InA_query_model/QueryFilterComparison.h"
#include "ModelSQLGenerator.h"
#include <string>

namespace query_generator
{
    std::string generateSQL(ina::query_model::QueryFilterComparison filter)
    {
        switch (filter.getComparisonOperator()) {
            case ina::query_model::QueryFilter::ComparisonOperator::IsNull:
            {
                return generateUnaryOperatorSQL(filter);
            }
            default:
            {
                return generateNAryOperatorSQL(filter);
            }        
        }
    }
    
    std::string generateNAryOperatorSQL(ina::query_model::QueryFilterComparison filter)
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
        filterSQL += ina::query_model::QueryFilter::comparisonOperatorToSqlString(filter.getComparisonOperator());
        // In case of unary operator, now low/high value
        if (!filter.getLowValue().empty())
        {
            filterSQL += " ";
            filterSQL += filter.getLowValue();
        }
        if (filter.isExcluding())
        {
            filterSQL += " )";
        }

        return filterSQL;
    }

    std::string generateUnaryOperatorSQL(ina::query_model::QueryFilterComparison filter)
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
        filterSQL += ina::query_model::QueryFilter::comparisonOperatorToSqlString(filter.getComparisonOperator());
        // In case of unary operator, now low/high value
        if (filter.isExcluding())
        {
            filterSQL += " )";
        }

        return filterSQL;
    }

    std::string generateSQL(ina::query_model::QuerySort querySort)
    {
        std::string sortSQL = querySort.getObjectName();
        
        if (querySort.getDirection() == ina::query_model::QuerySort::Direction::Ascending) 
        {
            sortSQL += " ASC";
        }
        else if (querySort.getDirection() == ina::query_model::QuerySort::Direction::Descending) 
        {
            sortSQL += " DESC";
        }

        return sortSQL;
    }
}