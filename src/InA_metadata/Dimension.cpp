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
        : _name(name), _description(description), _defaultaxe(defaultAxe)
    {
        _keyAttributes = 0;
        _attributes.clear();
        _attributes.push_back(Attribute(*this, name, description));
    }

    const std::string&  Dimension::getName() const
    {
        return _name;
    }

    const std::string&  Dimension::getUniqueName() const
    {
        return _name;
    }

    const std::string  Dimension::getNameExternal() const
    {
        std::string val = _description;
        #ifdef DEBUG
        val+=" (N.E.)";
        #endif
        return val;
    }

    const std::string  Dimension::getDescription() const
    {
        std::string val = _description;
        #ifdef DEBUG
        val+=" (D.)";
        #endif
        return val;
    }

    const std::vector<Attribute>& Dimension::getAttributes() const
    {
        return _attributes;
    }

    const Attribute& Dimension::getKeyAttribute() const
    {
        return _attributes.at(_keyAttributes);
    }

    const Attribute& Dimension::getTextAttribute() const
    {
        return _attributes.at(_textAttributes);
    }

    bool Dimension::haveTextAttribute() const
    {
        return _textAttributes >= 0;
    }

    const std::vector<Member>& Dimension::getMembers() const
    {
        return _members;
    }

    DimensionMeasures::DimensionMeasures(const std::string& name, const std::string& description, eAxe defaultAxe)
    : Dimension(name, description, defaultAxe)
    {
        _attributes.clear();

        _keyAttributes = 0; 
        _attributes.push_back(Attribute(*this, "[Measures].[Measures]", "[Measures].[Measures]"));

        _textAttributes = 1; 
        _attributes.push_back(Attribute(*this, "[Measures].[Name]", "[Measures].[Name]"));

        _members.push_back(Member(*this, {"[Measures].[Measures]", "Sales_revenue"}, {"[Measures].[Name]", "Sales revenue (N.)"}, {"[Measures].[Description]", "Sales revenue (D.)"}));
        _members.push_back(Member(*this, {"[Measures].[Measures]", "Quantity_sold"}, {"[Measures].[Name]", "Quantity sold (N.)"}, {"[Measures].[Description]", "Quantity sold (D.)"}));
        _members.push_back(Member(*this, {"[Measures].[Measures]", "Margin"},        {"[Measures].[Name]", "Margin Name (N.)"}, {"[Measures].[Description]", "Margin Name (D.)"}));
    }
}