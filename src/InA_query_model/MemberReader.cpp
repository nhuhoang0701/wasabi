#include "InA_query_model/Selection/Selection.h"
#include "Member.h"
#include "Formula.h"


#include <json/jsonReader.h>    // For JSONGenericObject
#include <memory>

namespace ina::query_model
{
	void read(MemberOperand& obj, const JSONGenericObject& jsonNode)
	{
		// MDS_TheDefinitveGuide_2_1 
		// MemberOperand is deprecated, but, in MANY examples
		obj.m_attributeName = jsonNode.getString("AttributeName");
		obj.m_value = jsonNode.getString("Value");
		obj.m_comparison = jsonNode.getString("Comparison");
	}

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
		
		if (const auto& selectionNode = jsonNode.getObject("Selection"))
		{
			obj.m_selection = std::make_unique<query_model::Selection>();
			read(*obj.m_selection, selectionNode);
		}
	
		if (JSONGenericObject memberOperandNode = jsonNode.getObject("MemberOperand")) 
		{
			// MDS_TheDefinitveGuide_2_1 
			// MemberOperand is deprecated, but, in MANY examples
			obj.m_memberOperand = std::make_unique<MemberOperand>();
			read(*obj.m_memberOperand, memberOperandNode);
		}
	}
}
