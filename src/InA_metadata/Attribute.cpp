#include "Attribute.h"

#include "Dimension.h"


namespace ina::metadata
{
    std::string toString(eDatatype dataType)
    {
        switch (dataType)
        {
        case eDatatype::Uninit : throw std::runtime_error("eDatatype value not init.");
        case eDatatype::String : return "String";
        case eDatatype::Long :return "Long";
        default:
            throw std::runtime_error("eDatatype unknow value");
        }
    }

    Attribute::Attribute(const Dimension& dimension, const std::string name, const std::string& description)
        : m_dimension(&dimension),m_name(name), m_description(description)
    {
    }

    const Dimension& Attribute::getDimension() const
    {
        return *m_dimension;
    }

    const std::string&  Attribute::getName() const
    {
        return m_name;
    }
    const std::string  Attribute::getNameExternal() const
    {
        return m_description + " att. (N.E.)";
    }
    const std::string  Attribute::getDescription() const
    {
        return m_description + " att. (D.)";
    }


	const std::string&   Attribute::getAlias() const
    {
        if(m_name[0] == '[')
            return m_name;
        const static std::string ID = "ID";
        return ID;
    }
	eDatatype            Attribute::getDatatype() const
    {
        //TODO
        return eDatatype::String;
    }
    const std::string&   Attribute::getSQLDataType() const
    {
        const static std::string SQLTYPE = "VARCHAR";
        return SQLTYPE;
    }
	eColumnType          Attribute::getColumnType() const
    {
        if(m_dimension->getDimensionType() != eDimType::MeasuresDimension)
            return eColumnType::NoMeasure;
        
        //TODO: certainly wrong
        return eColumnType::NoNumericMeasure;
    }
    bool  Attribute::isDisplayed() const
    {
        if(m_name=="[Measures].[Name]")
            return true;
        return false;
    }
    bool  Attribute::isFilterable() const
    {
        if(m_name == "[Measures].[Description]")
            return false;
        return true;
    }

    bool  Attribute::isKey() const
    {
        if(m_name=="[Measures].[Name]" || m_name=="[Measures].[Description]")
            return false;
        
        return true;
    }
    const std::string    Attribute::getPresentationType() const
    {
        if(isKey())
            return "Key";
        
        return "Text";
    }

    bool  Attribute::isNotNull() const
    {
        if(m_name[0]=='[')
            return false;
        return true;
    }

    std::uint32_t Attribute::getCardinality() const
    {
        if(m_name[0]=='[')
            return 0;
        
        return 10000;
    }
}