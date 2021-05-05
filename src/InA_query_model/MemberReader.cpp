#include "Member.h"
#include "Formula.h"


#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{
	void read(Member& obj, const JSONGenericObject& jsonNode)
	{
		if(jsonNode.haveValue("Aggregation"))
		{
			obj._aggregation = jsonNode.getString("Aggregation");
			obj._type = Member::eAggregation;
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
		else if(jsonNode.haveValue("Description"))
		{
			obj._name = jsonNode.getString("Description");
			JSONGenericObject formulaNode = jsonNode.getObject("Formula");

			if(formulaNode)
			{
				query_model::Formula formulaObj;
				read(formulaObj, formulaNode);

				obj._formula = formulaObj;	
				obj._type = Member::eFormula;			
			}

		}
	}
}
