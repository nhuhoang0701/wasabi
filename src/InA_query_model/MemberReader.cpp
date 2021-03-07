#include "Member.h"

#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{
	void read(Member& obj, const JSONGenericObject& jsonNode)
	{
		obj._aggregation = jsonNode.haveValue("Aggregation")? jsonNode.getString("Aggregation") : "";
		//TODO: Why this if ?
		obj._name = jsonNode.getObject("MemberOperand")? jsonNode.getObject("MemberOperand").getString("Name") : jsonNode.getString("Name");

	}
}
