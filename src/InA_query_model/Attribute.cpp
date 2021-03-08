#include "Attribute.h"
#include "Dimension.h"

#include "exceptions/InA_Exception.h"

namespace ina::query_model
{
    std::string toString(const ePresentationType& type)
    {
        switch (type)
        {
            case ePresentationType::Undefined:           return "Undefined";
            case ePresentationType::Key:                 return "Key";
            case ePresentationType::Text:                return "Text";
            case ePresentationType::HierarchyKey:        return "HierarchyKey";
            case ePresentationType::HierarchyText:       return "HierarchyText";
            case ePresentationType::HierarchyDisplayKey: return "HierarchyDisplayKey";
            default:
                break;
        }
        throw TRACED_InA_EXCEPTION("Unexpected presentation type");
    }

    static ePresentationType AttributeType2PresentationType(const Attribute::eType& attType)
    {
        switch (attType)
        {
            case Attribute::eType::Key:          return ePresentationType::Key;
            case Attribute::eType::Description:  return ePresentationType::Text;
            case Attribute::eType::Name:         return ePresentationType::Text;

            case Attribute::eType::HKey:         return ePresentationType::HierarchyKey;
            case Attribute::eType::HDescription: return ePresentationType::HierarchyText;
            case Attribute::eType::HName:        return ePresentationType::HierarchyDisplayKey;
            case Attribute::eType::HPath:        return ePresentationType::Undefined;

            case Attribute::eType::Generic:      return ePresentationType::Undefined;

            default:
                break;
        }
        throw TRACED_InA_EXCEPTION("Unexpected attribute type");
    }

    std::string Attribute::toString(eType attType)
    {
        switch (attType)
        {
            case Attribute::eType::Key:          return "Key";
            case Attribute::eType::Description:  return "Description";
            case Attribute::eType::Name:         return "Name";

            case Attribute::eType::HKey:         return "HierarchyKey";
            case Attribute::eType::HDescription: return "HierarchyDescription";
            case Attribute::eType::HName:        return "HierarchyName";
            case Attribute::eType::HPath:        return "HierarchyPath";

            case Attribute::eType::Generic:      return "Generic";

            default: break;
        }
        return "";
    }

    Attribute::Attribute(eType attType,
                                const Dimension& dim,
                                const std::string& name,
                                const std::string& description,
                                const std::string& alias,
                                //const InA_Types::DataType& dataType,
                                eFilterable flt,
                                eDisplayed displ)
        : m_dim(&dim),
        m_attType(attType),
        m_description(description),
        m_name(name),
        m_alias(alias),
        //m_dataType(dataType),
        m_presentationType(AttributeType2PresentationType(attType)),
        m_isFilterable(flt == IsFilterable),
        m_isDisplayed(displ == IsDisplayed)
    {
    }

    Attribute::~Attribute()
    {
        //m_dataType = InA_Types::UndefinedType;
        m_dim = nullptr;
    }

    Attribute::eType Attribute::getAttributeType() const
    {
        return m_attType;
    }

    const std::string& Attribute::getName() const
    {
        return m_name;
    }

    const std::string& Attribute::getDescription() const
    {
        return m_description;
    }

    const std::string& Attribute::getAlias() const
    {
        return m_alias;
    }

    /*model::Qualification Attribute::getQualification() const
    {
        return model::qAttribute;
    }*/

    const Dimension & Attribute::getDimension() const
    {
        if (m_dim == NULL)
            throw TRACED_InA_EXCEPTION("Attribute::getDimension missing dimension: " + m_name);
        return *m_dim;
    }

    /*InA_Types::DataType  Attribute::getDataType() const
    {
        return m_dataType;
    }*/

    ePresentationType Attribute::getPresentationType() const
    {
        return m_presentationType;
    }

    bool Attribute::isFilterable() const
    {
        return m_isFilterable;
    }

    bool  Attribute::isKey() const
    {
        return m_attType == Attribute::eType::Key
            || m_attType == Attribute::eType::HKey;
    }

    bool  Attribute::isFlatKey() const
    {
        return m_attType == Attribute::eType::Key;
    }

    bool  Attribute::isPath() const
    {
        return m_attType == Attribute::eType::HPath;
    }

    bool  Attribute::isDisplayed() const
    {
        return m_isDisplayed;
    }

    /*std::string Attribute::getDataTypeAsString() const
    {
        return InA_Types::toString(m_dataType);
    }

    int Attribute::getDataTypeAsInt() const
    {
        return m_dataType;
    }*/
}