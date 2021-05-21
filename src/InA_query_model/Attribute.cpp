#include "Attribute.h"
#include "Dimension.h"

#include <exceptions/InA_Exception.h>

namespace ina::query_model
{
    const std::string& Attribute::getName() const
    {
        return m_name;
    }

    const std::string& Attribute::getObtainability() const
    {
        return m_Obtainability;
    }
}
