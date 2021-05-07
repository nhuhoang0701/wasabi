#include "Dimension.h"
#include "InA_metadata/Member.h"


namespace ina::metadata
{
    Dimension::Dimension(const std::string& name, const std::string& description, eAxe defaultAxe)
        : _name(name), _description(description), _defaultaxe(defaultAxe)
    {
        _attributes.resize(1);

        Attribute keyAttribute(*this, _name, _description);
        _keyAttributes = 0;
        _attributes[_keyAttributes] = keyAttribute;
    }

    const std::string&  Dimension::getName() const
    {
        return _name;
    }

    const std::string&  Dimension::getUniqueName() const
    {
        return _name;
    }

    const std::string&  Dimension::getNameExternal() const
    {
        return _description;
    }

    const std::string&  Dimension::getDescription() const
    {
        return _description;
    }

    const std::vector<Attribute>& Dimension::getAttributes() const
    {
        return _attributes;
    }

    const Attribute& Dimension::getKeyAttribute() const
    {
        return _attributes[_keyAttributes];
    }

    const std::vector<Member>& Dimension::getMembers() const
    {
        return _members;
    }

    DimensionMeasures::DimensionMeasures(const std::string& name, const std::string& description, eAxe defaultAxe)
    : Dimension(name, description, defaultAxe)
    {
        _attributes.resize(2);

        Attribute keyAttribute(*this, "[Measures].[Measures]", "[Measures].[Measures]");
        _keyAttributes = 0; 
        _attributes[_keyAttributes] = keyAttribute;

        Attribute textAttribute(*this, "[Measures].[Name]", "[Measures].[Name]");
        _textAttributes = 1; 
        _attributes[_textAttributes] = textAttribute;

        _members.push_back(Member(*this, {"[Measures].[Measures]", "Sales_revenue"}, {"[Measures].[Name]", "Sales revenue"}, {"[Measures].[Description]", "Sales revenue"}));
        _members.push_back(Member(*this, {"[Measures].[Measures]", "Quantity_sold"}, {"[Measures].[Name]", "Quantity sold"}, {"[Measures].[Description]", "Quantity sold"}));
        _members.push_back(Member(*this, {"[Measures].[Measures]", "Margin"},        {"[Measures].[Name]", "Margin Name"}, {"[Measures].[Description]", "Margin Name"}));
    }
}