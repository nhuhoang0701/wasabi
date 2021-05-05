#include "Member.h"

#include <json/jsonReader.h>

namespace ina::query_model
{
	Member::Member(const std::string & name, const Aggregation & aggregation) :
	_name(name), _aggregation(aggregation), _type((eAggregation))
	{	
	}

	Member::Member(const std::string & name, const Formula & formula) :
	_name(name), _formula(formula),_type(eFormula)
	{

	}

	const Member::eMemberType Member::getType() const
	{
		return _type;
	}

	const std::string & Member::getName() const
	{
		return _name;
	}
	
	const Aggregation & Member::getAggregation() const
	{
		return _aggregation;
	}

	const Formula & Member::getFormula() const
	{
		return _formula;
	}
}
