#include "InA_queryFilter.h"
#include <vector>
namespace query_model
{
    class InA_queryFilterOperator : public InA_queryFilter {

        public:
            InA_queryFilterOperator(LogicalOperator logicalOperator);
            void addChild(const InA_queryFilter & child);

        private:
            InA_queryFilter::LogicalOperator _logicalOperator;
            std::vector<InA_queryFilter> _children;
    };
}