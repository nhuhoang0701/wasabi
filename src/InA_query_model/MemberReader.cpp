#include "Member.h"

#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{
	void read(Member& obj, const JSONGenericObject& jsonNode)
	{
		obj._aggregation = jsonNode.haveValue("Aggregation")? jsonNode.getString("Aggregation") : "";
		// MDS_TheDefinitveGuide_2_1 
		// MemberOperand is deprecated, but, in MANY examples
		if (jsonNode.getObject("MemberOperand")) 
		{
			std::string attributeName = jsonNode.getObject("MemberOperand").getString("AttributeName");
			if (attributeName == "Measures")
			{
				/*
				"MemberOperand": {
				"Comparison": "=",
				"AttributeName": "Measures",
				"Value": "SIGNEDDATA"
				}
				*/				
				obj._name = jsonNode.getObject("MemberOperand").getString("Value");
			}
			else
			{
				/*
				"MemberOperand": {
				"Comparison": "=",
				"AttributeName": "YEAR",
				"Value": "2015"
				}
				*/
				obj._name = jsonNode.getObject("MemberOperand").getString("AttributeName");
			}
		}
		else
		{
			/*
			"Member": {
			"Name": "SD_2015"
			}
			*/
			obj._name = jsonNode.getString("Name");
		}
	}
}
