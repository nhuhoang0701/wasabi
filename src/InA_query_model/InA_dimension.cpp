#include "InA_dimension.h"

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
	
	const std::vector<InA_member> & Dimension::getMembers() const
	{
		return _members;
	}

	void Dimension::addMember(const InA_member & member) 
	{
		_members.push_back(member);
	}
}