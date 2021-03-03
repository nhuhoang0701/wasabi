#include "InA_dimension.h"
namespace query_model
{
	InA_dimension::InA_dimension(const std::string & name, const Type & type, const Datatype & datatype) 
	: _name(name), _type(type), _datatype(datatype)
	{
		
	}
	
	const std::string & InA_dimension::getName() const
	{
		return _name;
	}
	
	const Datatype & InA_dimension::getDatatype() const
	{
		return _datatype;
	}
	const InA_dimension::Type & InA_dimension::getType() const
	{
		return _type;
	}
	
	const std::vector<InA_member> & InA_dimension::getMembers() const
	{
		return _members;
	}

	void InA_dimension::addMember(InA_member & member) 
	{
		_members.push_back(member);
	}
}