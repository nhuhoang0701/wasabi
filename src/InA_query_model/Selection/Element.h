#pragma once
#include <common/data.h>

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

            static ComparisonOperator getComparisonOperator(const std::string& str);

            static std::string toString(const Element::ComparisonOperator& comparator);

            static std::string toSql(const Element::ComparisonOperator& comparator);

            Element(const std::string& fieldName);

            void setComparisonOperator(Element::ComparisonOperator comparisonOperator);
            void setLowValue(const common::Value& lowValue);
            void setHighValue(const common::Value& highValue);
            void setExcluding(const bool isExcluding);

            Element::ComparisonOperator getComparisonOperator() const;
            const std::string& getFieldName() const;
            const common::Value& getLowValue() const;
            const common::Value& getHighValue() const;
            const bool isExcluding() const;
        private:
            std::string _fieldName;
            Element::ComparisonOperator _comparisonOperator;
            common::Value _lowValue;
            common::Value _highValue;
            bool _isExcluding;

            friend void read(std::vector<Element> & elements, JSONGenericObject setOperandNode);
    };
}
