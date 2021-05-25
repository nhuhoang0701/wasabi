#include "Element.h"
#include <string>

namespace ina::query_model 
{

        Element::ComparisonOperator Element::getComparisonOperator(const std::string& str)
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

        std::string Element::toSql(const Element::ComparisonOperator& comparator)
        {
            if (comparator == ComparisonOperator::EqualTo)                          return "=";
            else if (comparator == ComparisonOperator::NotEqualTo)                  return "<>";
            else if (comparator == ComparisonOperator::Between)                     return"BETWEEN";
            else if (comparator == ComparisonOperator::Match)                       return "MATCH";
            else if (comparator == ComparisonOperator::GreaterThan)                  return ">";
            else if (comparator == ComparisonOperator::GreaterThanOrEqualTo)        return ">=";
            else if (comparator == ComparisonOperator::IsNull)                      return "IS NULL";
            else if (comparator == ComparisonOperator::LessThan)                    return "<";
            else if (comparator == ComparisonOperator::LessThanOrEqualTo)           return "<=";
            else return "";
        };

        std::string Element::toString(const Element::ComparisonOperator& comparator)
        {
            if (comparator == ComparisonOperator::EqualTo)                          return "=";
            else if (comparator == ComparisonOperator::NotEqualTo)                  return "<>";
            else if (comparator == ComparisonOperator::Between)                     return"BETWEEN";
            else if (comparator == ComparisonOperator::Match)                       return "MATCH";
            else if (comparator == ComparisonOperator::GreaterThan)                  return ">";
            else if (comparator == ComparisonOperator::GreaterThanOrEqualTo)        return ">=";
            else if (comparator == ComparisonOperator::IsNull)                      return "IS_NULL";
            else if (comparator == ComparisonOperator::LessThan)                    return "<";
            else if (comparator == ComparisonOperator::LessThanOrEqualTo)           return "<=";
            else return "";
        };

        void Element::setComparisonOperator(Element::ComparisonOperator comparisonOperator)
        {
            _comparisonOperator = comparisonOperator;
        }

        void Element::setLowValue(const common::Value& lowValue)
        {
            _lowValue = lowValue;
        }

        void Element::setHighValue(const common::Value& highValue)
        {
            _highValue = highValue;
        }

        void Element::setExcluding(const bool isExcluding)
        {
            _isExcluding = isExcluding;
        }

        Element::ComparisonOperator Element::getComparisonOperator() const
        {
            return _comparisonOperator;
        }

        const common::Value& Element::getLowValue() const
        {
            return _lowValue;
        }

        const common::Value& Element::getHighValue() const
        {
            return _highValue;
        }

        const bool Element::isExcluding() const 
        {
            return _isExcluding;
        }
}
