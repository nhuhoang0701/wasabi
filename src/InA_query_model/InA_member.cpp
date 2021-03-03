#include "InA_member.h"
			
namespace query_model
{
	InA_member::InA_member(const std::string & name, const Datatype & datatype, const Aggregation & aggregation) :
	_name(name), _datatype(datatype), _aggregation(aggregation)
	{
		
	}
	
	const std::string & InA_member::getName() const
	{
		return _name;
	}
	
	const Datatype & InA_member::getDatatype() const
	{
		return _datatype;
	}
	const Aggregation & InA_member::getAggregation() const
	{
		return _aggregation;
	}
}			