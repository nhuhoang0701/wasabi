#pragma once
#include <string>
#include <vector>

class JSONGenericObject; // From <json/jsonReader.h>

namespace ina::query_model 
{
	class Element
    {
        public:
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

            inline static ComparisonOperator getComparisonOperator(const std::string& str)
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

            inline static std::string toString(const Element::ComparisonOperator& comparator)
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

            inline static std::string toSql(const Element::ComparisonOperator& comparator)
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

            Element(const std::string& fieldName);

            void setComparisonOperator(Element::ComparisonOperator comparisonOperator);
            void setLowValue(const std::string & lowValue);
            void setHighValue(const std::string& highValue);
            void setExcluding(const bool isExcluding);

            Element::ComparisonOperator getComparisonOperator() const;
            const std::string& getFieldName() const;
            const std::string& getLowValue() const;
            const std::string& getHighValue() const;
            const bool isExcluding() const;
        private:
            std::string _fieldName;
            Element::ComparisonOperator _comparisonOperator;
            std::string _lowValue;
            std::string _highValue;
            bool _isExcluding;

            friend void read(std::vector<Element> & elements, JSONGenericObject setOperandNode);
    };
}
