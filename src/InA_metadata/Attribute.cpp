#include "Attribute.h"


namespace ina::metadata
{
    Attribute::Attribute(const Dimension& dimension, const std::string name, const std::string& description)
        : m_dimension(&dimension)
    {
    }

    const Dimension& Attribute::getDimension() const
    {
        return *m_dimension;
    }
}