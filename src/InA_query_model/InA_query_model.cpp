#include "InA_query_model.h"

#include <iostream>

namespace query_model
{
    void InA_query_model::addDimension(const InA_dimension& dimension)
    {
        m_objs.push_back(dimension);
    }
    const std::vector<InA_member>& InA_query_model::getVisibleMembers(const InA_dimension& dimension) const 
    {
        return dimension.getMembers();
    }

} //query_model