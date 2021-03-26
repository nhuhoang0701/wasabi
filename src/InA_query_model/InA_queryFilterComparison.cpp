#include "InA_queryFilterComparison.h"
#include <string>

namespace ina::query_model 
{
        InA_queryFilterComparison::InA_queryFilterComparison(const std::string& fieldName) 
            : _fieldName(fieldName),_comparisonOperator(InA_queryFilter::ComparisonOperator::Unknown),_isExcluding(false)
        {

        }

        void InA_queryFilterComparison::setComparisonOperator(InA_queryFilter::ComparisonOperator comparisonOperator)
        {
            _comparisonOperator = comparisonOperator;
        }

        void InA_queryFilterComparison::setLowValue(const std::string & lowValue)
        {
            _lowValue = lowValue;
        }

        void InA_queryFilterComparison::setHighValue(const std::string& highValue)
        {
            _highValue = highValue;
        }

        void InA_queryFilterComparison::setExcluding(const bool isExcluding)
        {
            _isExcluding = isExcluding;
        }

        InA_queryFilter::ComparisonOperator InA_queryFilterComparison::getComparisonOperator() const
        {
            return _comparisonOperator;
        }

        const std::string& InA_queryFilterComparison::getFieldName() const
        {
            return _fieldName;
        }

        const std::string& InA_queryFilterComparison::getLowValue() const
        {
            return _lowValue;
        }

        const std::string& InA_queryFilterComparison::getHighValue() const
        {
            return _highValue;
        }

        const bool InA_queryFilterComparison::isExcluding() const 
        {
            return _isExcluding;
        }
}
