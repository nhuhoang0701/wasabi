#include "Dimension.h"
#include "InA_metadata/Member.h"
#include <stdexcept>


namespace ina::metadata
{
    std::string toString(eAxe axe)
    {
        switch (axe)
        {
        case eAxe::Uninit : throw std::runtime_error("eAxe value not init.");
        case eAxe::Free : return "Free";
        case eAxe::Rows :return "Rows";
        case eAxe::Columns : return "Columns";
        default:
            throw std::runtime_error("eAxe unknow value");
        }
    }

    Dimension::Dimension(const std::string& name, const std::string& description, eAxe defaultAxe)
        : m_name(name), m_description(description), m_defaultaxe(defaultAxe)
    {
    }

    const std::string&  Dimension::getName() const
    {
        return m_name;
    }
    const std::string&  Dimension::getUniqueName() const
    {
        return m_name;
    }
    const std::string  Dimension::getNameExternal() const
    {
        std::string val = m_description;
        #ifdef DEBUG
        val+=" (N.E.)";
        #endif
        return val;
    }

    const std::string  Dimension::getDescription() const
    {
        std::string val = m_description;
        #ifdef DEBUG
        val+=" (D.)";
        #endif
        return val;
    }

    void Dimension::addKeyAttribute(const Attribute &att)
    {
        if(m_keyAttributes != -1)
            throw std::runtime_error("Dimension::addKeyAttribute key attribut already exist");

        m_keyAttributes = m_attributes.size();
        addAttribute(att);
    }

    void Dimension::addTextAttribute(const Attribute &att)
    {
        if(m_textAttributes != -1)
            throw std::runtime_error("Dimension::addTextAttribute text attribut already exist");

        m_textAttributes = m_attributes.size();
        addAttribute(att);
    }

    void Dimension::addAttribute(const Attribute &att)
    {
        if(&att.getDimension() != this)
            throw std::runtime_error("Dimension::addAttribute attribut is not onw by this dimension");
        m_attributes.push_back(att);
    }

    const std::vector<Attribute>& Dimension::getAttributes() const
    {
        return m_attributes;
    }

    const Attribute& Dimension::getKeyAttribute() const
    {
        return m_attributes.at(m_keyAttributes);
    }

    const Attribute& Dimension::getTextAttribute() const
    {
        return m_attributes.at(m_textAttributes);
    }

    bool Dimension::haveTextAttribute() const
    {
        return m_textAttributes >= 0;
    }

    const std::vector<Member>& Dimension::getMembers() const
    {
        return m_members;
    }

   void  Dimension::addMember(const Member& member)
   {
       m_members.push_back(member);
   }

    DimensionMeasures::DimensionMeasures(const std::string& name, const std::string& description, eAxe defaultAxe)
    : Dimension(name, description, defaultAxe)
    {
        m_attributes.clear();

        m_keyAttributes = 0; 
        m_attributes.push_back(Attribute(*this, "[Measures].[Measures]", "[Measures].[Measures]", eAttrType::Key));

        m_textAttributes = 1; 
        m_attributes.push_back(Attribute(*this, "[Measures].[Name]", "[Measures].[Name]", eAttrType::Text));
   }
}