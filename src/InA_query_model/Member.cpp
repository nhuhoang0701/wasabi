#include "Member.h"

#include <json/jsonReader.h>

namespace ina::query_model
{
	Member::Member(const std::string & name, const Aggregation & aggregation) :
	_name(name), _aggregation(aggregation)
	{	
	}

	const std::string & Member::getName() const
	{
		return _name;
	}
	
	const Aggregation & Member::getAggregation() const
	{
		return _aggregation;
	}
}
