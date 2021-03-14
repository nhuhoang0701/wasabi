#include "Attribute.h"
#include "Dimension.h"

#include <exceptions/InA_Exception.h>

namespace ina::query_model
{
    Attribute::Attribute( const std::string& name,
                          const std::string& Obtainability)
        : m_name(name),
          m_Obtainability(Obtainability),
          m_dim(nullptr)
    {
    }

    Attribute::~Attribute()
    {
        m_dim = nullptr;
    }

    const std::string& Attribute::getName() const
    {
        return m_name;
    }

    const Dimension& Attribute::getDimension() const
    {
        if(m_dim == nullptr)
            throw TRACED_InA_EXCEPTION("Attribut not attach to a dimension");
        return *m_dim;
    }

    const std::string& Attribute::getObtainability() const
    {
        return m_Obtainability;
    }
}
