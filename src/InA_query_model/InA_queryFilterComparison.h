#pragma once
#include "InA_queryFilter.h"
#include <string>

namespace ina::query_model 
{
	class InA_queryFilterComparison : public InA_queryFilter
    {
        public:

            InA_queryFilterComparison(const std::string& fieldName);

            void setComparisonOperator(InA_queryFilter::ComparisonOperator comparisonOperator);
            void setLowValue(const std::string & lowValue);
            void setHighValue(const std::string& highValue);

            InA_queryFilter::ComparisonOperator getComparisonOperator() const;
            const std::string& getFieldName() const;
            const std::string& getLowValue() const;
            const std::string& getHighValue() const;
        private:
            std::string _fieldName;
            InA_queryFilter::ComparisonOperator _comparisonOperator;
            std::string _lowValue;
            std::string _highValue;
    };
}
