#include "InA_dimension.h"

namespace ina::query_model
{
	InA_dimension::InA_dimension(const std::string & name, const std::string & axename) 
	: _name(name), _axename(axename)
	{	
	}
	
	const std::string & InA_dimension::getName() const
	{
		return _name;
	}
	
	const std::vector<InA_member> & InA_dimension::getMembers() const
	{
		return _members;
	}

	void InA_dimension::addMember(const InA_member & member) 
	{
		_members.push_back(member);
	}
}