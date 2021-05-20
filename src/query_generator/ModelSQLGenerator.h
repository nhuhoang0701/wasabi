#include "InA_query_model/QuerySort.h"
#include "InA_query_model/Selection/Element.h"
#include <string>
namespace query_generator
{
    std::string generateSQL(const ina::query_model::Element& filter);
    std::string generateNAryOperatorSQL(const ina::query_model::Element& filter);
    std::string generateUnaryOperatorSQL(const ina::query_model::Element& filter);

    std::string generateSQL(const ina::query_model::QuerySort& querySort);
}