#pragma once
#include "QueryFilter.h"
#include <string>

namespace ina::query_model 
{
	class QueryFilterComparison : public QueryFilter
    {
        public:

            QueryFilterComparison(const std::string& fieldName);

            void setComparisonOperator(QueryFilter::ComparisonOperator comparisonOperator);
            void setLowValue(const std::string & lowValue);
            void setHighValue(const std::string& highValue);
            void setExcluding(const bool isExcluding);

            QueryFilter::ComparisonOperator getComparisonOperator() const;
            const std::string& getFieldName() const;
            const std::string& getLowValue() const;
            const std::string& getHighValue() const;
            const bool isExcluding() const;
        private:
            std::string _fieldName;
            QueryFilter::ComparisonOperator _comparisonOperator;
            std::string _lowValue;
            std::string _highValue;
            bool _isExcluding;
    };
}
