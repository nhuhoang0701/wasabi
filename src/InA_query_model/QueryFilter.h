#pragma once

#include <string>

namespace ina::query_model
{
    class QueryFilter
	{
        public:
            enum class LogicalOperator
            {
                Or,
                And,
                Not,
                Undefined
            };

            enum class ComparisonOperator
            {
                EqualTo,
                NotEqualTo,
                Between,
                GreaterThan,
                GreaterThanOrEqualTo,
                IsNull,
                LessThan,
                LessThanOrEqualTo,
                Match,
                Unknown
            };

            inline static QueryFilter::ComparisonOperator getComparisonOperator(const std::string& str)
            {
                if (str == "=") 		     return ComparisonOperator::EqualTo;
                else if (str == "<>")		 return ComparisonOperator::NotEqualTo;
                else if (str == "BETWEEN")   return ComparisonOperator::Between;
                else if (str == "MATCH")     return ComparisonOperator::Match;
                else if (str == ">")         return ComparisonOperator::GreaterThan;
                else if (str == ">=")        return ComparisonOperator::GreaterThanOrEqualTo;
                else if (str == "IS_NULL")   return ComparisonOperator::IsNull;
                else if (str == "<")         return ComparisonOperator::LessThan;
                else if (str == "<=")        return ComparisonOperator::LessThanOrEqualTo;
                else                         return ComparisonOperator::Unknown;
            };

            inline static std::string comparisonOperatorToString(const QueryFilter::ComparisonOperator& comparator)
            {
                if (comparator == ComparisonOperator::EqualTo)                          return "=";
                else if (comparator == ComparisonOperator::NotEqualTo)                  return "<>";
                else if (comparator == ComparisonOperator::Between)                     return"BETWEEN";
                else if (comparator == ComparisonOperator::Match)                       return "MATCH";
                else if (comparator ==ComparisonOperator::GreaterThan)                  return ">";
                else if (comparator == ComparisonOperator::GreaterThanOrEqualTo)        return ">=";
                else if (comparator == ComparisonOperator::IsNull)                      return "IS_NULL";
                else if (comparator == ComparisonOperator::LessThan)                    return "<";
                else if (comparator == ComparisonOperator::LessThanOrEqualTo)           return "<=";
                else return "";
            };

            inline static std::string comparisonOperatorToSqlString(const QueryFilter::ComparisonOperator& comparator)
            {
                if (comparator == ComparisonOperator::EqualTo)                          return "=";
                else if (comparator == ComparisonOperator::NotEqualTo)                  return "<>";
                else if (comparator == ComparisonOperator::Between)                     return"BETWEEN";
                else if (comparator == ComparisonOperator::Match)                       return "MATCH";
                else if (comparator ==ComparisonOperator::GreaterThan)                  return ">";
                else if (comparator == ComparisonOperator::GreaterThanOrEqualTo)        return ">=";
                else if (comparator == ComparisonOperator::IsNull)                      return "IS NULL";
                else if (comparator == ComparisonOperator::LessThan)                    return "<";
                else if (comparator == ComparisonOperator::LessThanOrEqualTo)           return "<=";
                else return "";
            };

            QueryFilter() = default;
    };   
}