#include "Member.h"
#include "Formula.h"


#include <json/jsonReader.h>    // For JSONGenericObject
#include <memory>

namespace ina::query_model
{
	void read(Member& obj, const JSONGenericObject& jsonNode)
	{
		if(jsonNode.haveValue("Aggregation"))
			obj.m_aggregation = jsonNode.getString("Aggregation");

		if(jsonNode.haveValue("Name"))
			obj.m_name = jsonNode.getString("Name");

		if(jsonNode.haveValue("Description"))
			obj.m_description = jsonNode.getString("Description");
						
		if(JSONGenericObject formulaNode = jsonNode.getObject("Formula"))
		{
			obj.m_formula = std::make_unique<query_model::Formula>();
			read(*obj.m_formula, formulaNode);		
		}
	
		// MDS_TheDefinitveGuide_2_1 
		// MemberOperand is deprecated, but, in MANY examples
		if (JSONGenericObject memberOperandNode = jsonNode.getObject("MemberOperand")) 
		{
			obj.m_memberOperand = std::make_unique<MemberOperand>();
			obj.m_memberOperand->m_attributeName = memberOperandNode.getString("AttributeName");
			obj.m_memberOperand->m_value = memberOperandNode.getString("Value");
			obj.m_memberOperand->m_comparison = memberOperandNode.getString("Comparison");
		}
	}
}
