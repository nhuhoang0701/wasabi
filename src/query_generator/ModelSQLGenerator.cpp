#include "ModelSQLGenerator.h"
#include <stdexcept>
#include <string>

namespace query_generator
{
    
	std::string_view toSql(const ina::query_model::Element::ComparisonOperator& comparator)
	{
		if (comparator == ina::query_model::Element::ComparisonOperator::EqualTo)                          return "=";
		else if (comparator == ina::query_model::Element::ComparisonOperator::NotEqualTo)                  return "<>";
		else if (comparator == ina::query_model::Element::ComparisonOperator::Between)                     return"BETWEEN";
		else if (comparator == ina::query_model::Element::ComparisonOperator::Match)                       return "MATCH";
		else if (comparator == ina::query_model::Element::ComparisonOperator::GreaterThan)                  return ">";
		else if (comparator == ina::query_model::Element::ComparisonOperator::GreaterThanOrEqualTo)        return ">=";
		else if (comparator == ina::query_model::Element::ComparisonOperator::IsNull)                      return "IS NULL";
		else if (comparator == ina::query_model::Element::ComparisonOperator::LessThan)                    return "<";
		else if (comparator == ina::query_model::Element::ComparisonOperator::LessThanOrEqualTo)           return "<=";
		else throw std::runtime_error("unknow Element::ComparisonOperator");
	};
    
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
        filterSQL += toSql(filter.getComparisonOperator());
        // In case of unary operator, now low/high value
        std::string valueStr;
        if(filter.getLowValue().isString())
            valueStr = filter.getLowValue().getString();
        else if(filter.getLowValue().isDouble())
            valueStr = std::to_string(filter.getLowValue().getDouble());
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
        filterSQL += toSql(filter.getComparisonOperator());
        // In case of unary operator, now low/high value
        if (filter.isExcluding())
        {
            filterSQL += " )";
        }

        return filterSQL;
    }
}