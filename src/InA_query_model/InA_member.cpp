#include "InA_member.h"

#include <json/jsonReader.h>

namespace ina::query_model
{
	InA_member::InA_member(const std::string & name, const Aggregation & aggregation) :
	_name(name), _aggregation(aggregation)
	{	
	}

	const std::string & InA_member::getName() const
	{
		return _name;
	}
	
	const Aggregation & InA_member::getAggregation() const
	{
		return _aggregation;
	}
}
