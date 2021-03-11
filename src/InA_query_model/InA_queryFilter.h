#pragma once

#include <string>

namespace ina::query_model
{
    class InA_queryFilter
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

            inline static InA_queryFilter::ComparisonOperator getComparator(const std::string& str)
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

            InA_queryFilter() = default;
    };   
}