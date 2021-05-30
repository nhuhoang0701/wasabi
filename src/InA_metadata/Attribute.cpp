#include "Attribute.h"

#include "Dimension.h"

#include <exception>


namespace ina::metadata
{
    std::string_view toString(common::eDataType dataType)
    {
        switch (dataType)
        {
        case common::eDataType::Undefined : throw std::runtime_error("eDatatype value not init.");
        case common::eDataType::String : return "String";
        case common::eDataType::Numeric :return "Long";
        default:
            throw std::runtime_error("eDatatype unknow value");
        }
    }
    Attribute::Attribute(const Dimension& dimension, const std::string name, const std::string& description, eAttrType type, common::eDataType datatype)
        : m_dimension(&dimension),m_name(name), m_description(description), m_datatype(datatype), m_type(type)
    {
    }

	Attribute::Attribute(const Attribute& other)
    {
        m_dimension = other.m_dimension;
        m_name = other.m_name;
        m_description = other.m_description;
        m_datatype = other.m_datatype;
        m_type = other.m_type;
    }
	Attribute& Attribute::operator=(const Attribute& other)
    {
        m_dimension = other.m_dimension;
        m_name = other.m_name;
        m_description = other.m_description;
        m_datatype = other.m_datatype;
        m_type = other.m_type;

        return *this;
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
        std::string val = m_description;
        #ifdef DEBUG
        val+=" att. (N.E.)";
        #endif
        return val;
    }
    const std::string  Attribute::getDescription() const
    {
        std::string val = m_description;
        #ifdef DEBUG
        val+=" att. (D.)";
        #endif
        return val;
    }


	const std::string&   Attribute::getAlias() const
    {
        if(m_name[0] == '[')
            return m_name;
        const static std::string ID = "ID";
        return ID;
    }
	common::eDataType Attribute::getDataType() const
    {
        return m_datatype;
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
        return m_type==eAttrType::Key;
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