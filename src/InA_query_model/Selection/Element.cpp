#include "Element.h"
#include <string>

namespace ina::query_model 
{
        Element::Element(const std::string& fieldName) 
            : _fieldName(fieldName),_comparisonOperator(Element::ComparisonOperator::Unknown),_isExcluding(false)
        {

        }

        void Element::setComparisonOperator(Element::ComparisonOperator comparisonOperator)
        {
            _comparisonOperator = comparisonOperator;
        }

        void Element::setLowValue(const std::string & lowValue)
        {
            _lowValue = lowValue;
        }

        void Element::setHighValue(const std::string& highValue)
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

        const std::string& Element::getFieldName() const
        {
            return _fieldName;
        }

        const std::string& Element::getLowValue() const
        {
            return _lowValue;
        }

        const std::string& Element::getHighValue() const
        {
            return _highValue;
        }

        const bool Element::isExcluding() const 
        {
            return _isExcluding;
        }
}
