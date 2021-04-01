#include "QueryFilterComparison.h"
#include <string>

namespace ina::query_model 
{
        QueryFilterComparison::QueryFilterComparison(const std::string& fieldName) 
            : _fieldName(fieldName),_comparisonOperator(QueryFilter::ComparisonOperator::Unknown),_isExcluding(false)
        {

        }

        void QueryFilterComparison::setComparisonOperator(QueryFilter::ComparisonOperator comparisonOperator)
        {
            _comparisonOperator = comparisonOperator;
        }

        void QueryFilterComparison::setLowValue(const std::string & lowValue)
        {
            _lowValue = lowValue;
        }

        void QueryFilterComparison::setHighValue(const std::string& highValue)
        {
            _highValue = highValue;
        }

        void QueryFilterComparison::setExcluding(const bool isExcluding)
        {
            _isExcluding = isExcluding;
        }

        QueryFilter::ComparisonOperator QueryFilterComparison::getComparisonOperator() const
        {
            return _comparisonOperator;
        }

        const std::string& QueryFilterComparison::getFieldName() const
        {
            return _fieldName;
        }

        const std::string& QueryFilterComparison::getLowValue() const
        {
            return _lowValue;
        }

        const std::string& QueryFilterComparison::getHighValue() const
        {
            return _highValue;
        }

        const bool QueryFilterComparison::isExcluding() const 
        {
            return _isExcluding;
        }
}
