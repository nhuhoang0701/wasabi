#include "Dimension.h"

namespace ina::query_model
{
	Dimension::Dimension(const std::string & name, const std::string & axename) 
	: _name(name), _axename(axename)
	{	
	}
	
	const std::string & Dimension::getName() const
	{
		return _name;
	}
	
	const std::vector<Member> & Dimension::getMembers() const
	{
		return _members;
	}

	void Dimension::addMember(const Member & member) 
	{
		_members.push_back(member);
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