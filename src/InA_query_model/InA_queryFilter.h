

namespace query_model
{
    class InA_queryFilter {
        public:
            enum LogicalOperator
            {
                Or,
                And,
                Not,
                Undefined
            };

            enum ComparisonOperator
            {
                EqualTo,
                NotEqualTo,
                Between,
                GreaterThan,
                GreaterThanOrEqualTo,
                IsNull,
                LessThan,
                LessThanOrEqualTo,
                Match
            };

            InA_queryFilter();
    };   
}