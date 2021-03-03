#include "InA_query_model.h"

#include <iostream>

namespace query_model
{
    void InA_query_model::addDimension(const InA_dimension& dimension)
    {
        m_objs.push_back(dimension);
    }

} //query_model