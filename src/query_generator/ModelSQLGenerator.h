#include <InA_query_model/Sort.h>
#include <InA_query_model/Selection/Element.h>

#include <string>

namespace query_generator
{
    std::string generateSQL(const std::string& fieldName, const ina::query_model::Element& filter);

    std::string generateSQL(const ina::query_model::Sort& querySort);
}