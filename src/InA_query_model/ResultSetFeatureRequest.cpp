#include "ResultSetFeatureRequest.h"

#include <string>

namespace ina::query_model
{
    const SubSetDescription& ResultSetFeatureRequest::getSubSetDescription() const
    {
        return m_SubSetDescription;
    }
}