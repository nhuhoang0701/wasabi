#include "Dimension.h"

namespace ina::query_model
{
	
	const std::string Dimension::DIMENSION_OF_MEASURES_NAME = "CustomDimension1";

	Dimension::Dimension(const std::string & name, eAxe axe) 
	: _name(name), _axe(axe)
	{	
	}
	
	const std::string & Dimension::getName() const
	{
		return _name;
	}

	Dimension::eAxe Dimension::getAxe() const
	{
		return _axe;
	}
	
	const std::vector<Member> & Dimension::getMembers() const
	{
		return _members;
	}

	void Dimension::addMember(const Member & member) 
	{
		_members.push_back(member);
	}

	const Attribute& Dimension::getAttribute(const std::string& name) const
	{
         for(const auto& attribut  : _attributes)
            if(attribut.getName() == name)
                return attribut;
         
         throw std::runtime_error("Dimension::getAttribute : attribut not found: " + name);
	}
	bool Dimension::haveAttribute(const std::string& name) const
	{
         for(const auto& attribut  : _attributes)
            if(attribut.getName() == name)
                return true;
         
         return false;
	}
	const std::vector<Attribute>& Dimension::getAttributes()  const 
	{
		return _attributes;
	}

	void Dimension::addAttribute(const Attribute& att) 
	{
		_attributes.push_back(att);
	}
}