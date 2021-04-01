#include "InA_query_model/QueryFilterComparison.h"
#include "InA_query_model/QuerySort.h"
#include <string>
namespace query_generator
{
    std::string generateSQL(ina::query_model::QueryFilterComparison filter);
    std::string generateNAryOperatorSQL(ina::query_model::QueryFilterComparison filter);
    std::string generateUnaryOperatorSQL(ina::query_model::QueryFilterComparison filter);

    std::string generateSQL(ina::query_model::QuerySort querySort);
}