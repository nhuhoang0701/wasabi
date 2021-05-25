#include "ModelSQLGenerator.h"
#include <stdexcept>
#include <string>

namespace query_generator
{
    
    std::string generateNAryOperatorSQL(const std::string& fieldName, const ina::query_model::Element& filter);
    std::string generateUnaryOperatorSQL(const std::string& fieldName, const ina::query_model::Element& filter);

    std::string generateSQL(const std::string& fieldName, const ina::query_model::Element& filter)
    {
        switch (filter.getComparisonOperator()) {
            case ina::query_model::Element::ComparisonOperator::IsNull:
            {
                return generateUnaryOperatorSQL(fieldName, filter);
            }
            default:
            {
                return generateNAryOperatorSQL(fieldName, filter);
            }        
        }
    }
    
    std::string generateNAryOperatorSQL(const std::string& fieldName, const ina::query_model::Element& filter)
    {
        std::string filterSQL;
        if (filter.isExcluding())
        {
            filterSQL += "NOT ( ";
        }
        // fieldName is never empty 
        filterSQL += fieldName;
        filterSQL += " " ;
        // comparisonOperator is never empty 
        filterSQL += ina::query_model::Element::toSql(filter.getComparisonOperator());
        // In case of unary operator, now low/high value
        std::string valueStr;
        if(std::holds_alternative<std::string>(filter.getLowValue()))
            valueStr = std::get<std::string>(filter.getLowValue());
        else if(std::holds_alternative<double>(filter.getLowValue()))
            valueStr = std::to_string(std::get<double>(filter.getLowValue()));
        else
            throw std::runtime_error("common::Value : a datatype is NYI.");
        if (!valueStr.empty())
        {
            filterSQL += " '";
            filterSQL += valueStr;
            filterSQL += "'";
        }
        if (filter.isExcluding())
        {
            filterSQL += " )";
        }

        return filterSQL;
    }

    std::string generateUnaryOperatorSQL(const std::string& fieldName, const ina::query_model::Element& filter)
    {
        std::string filterSQL;
        if (filter.isExcluding())
        {
            filterSQL += "NOT ( ";
        }
        // fieldName is never empty 
        filterSQL += fieldName;
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