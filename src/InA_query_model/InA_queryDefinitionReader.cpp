#include "InA_queryDefinition.h"

#include <json/jsonReader.h>
#include <exceptions/InA_Exception.h>

#include <iostream>

namespace ina::query_model
{
    void read(Definition & definition, const JSONGenericObject& definitionNode)
	{
		if (definitionNode.isString("ValuehelpForVariable"))
		{
			throw TRACED_InA_EXCEPTION("ValuehelpForVariable NYI");
		}

		if (JSONGenericObject variables = definitionNode.getArray("Variables"))
		{
			throw TRACED_InA_EXCEPTION("Variables NYI");
		}
		if(const auto& dims = definitionNode.getArray("Dimensions"))
		{
			for(int i = 0;i < dims.size();i++)
			{
				const auto& dim = dims[i];
				query_model::InA_dimension dimensionObj(dim.getString("Name"), dim.getString("Axis"));

				if(const auto& members = dim.getArray("Members"))
				{
					for(int i = 0;i < members.size();i++)
					{
						const auto& member = members[i];
						const std::string agg = member.haveValue("Aggregation")? member.getString("Aggregation") : "";
						const std::string name = member.getObject("MemberOperand")? member.getObject("MemberOperand").getString("Name") : member.getString("Name");
						dimensionObj.addMember(query_model::InA_member(name, agg));
					}
				}
				definition.addDimension(dimensionObj);
			}
		}
		if(const auto& subSelections = definitionNode.getObject("DynamicFilter").getObject("Selection").getObject("Operator").getArray("SubSelections"))
		{
			for(size_t i = 0; i < subSelections.size(); ++i)
			{
				const std::string fieldName = subSelections[i].getObject("SetOperand").getString("FieldName");
				if(const auto& elements = subSelections[i].getObject("SetOperand").getArray("Elements"))
				{
					for(size_t i = 0; i < elements.size(); ++i)
					{	
						query_model::InA_queryFilterComparison queryFilterComparison(fieldName);
						std::string lowValue = elements[i].getString("Low");
						queryFilterComparison.setLowValue(lowValue);

						query_model::InA_queryFilter::ComparisonOperator comparisonOperator = query_model::InA_queryFilter::getComparator(elements[i].getString("Comparison"));
						queryFilterComparison.setComparisonOperator(comparisonOperator);
						definition.addQueryFilter(queryFilterComparison);
						std::cout << "InA_Interpreter => DynamicFilter -> ... -> Low :" << lowValue << std::endl;
					}
				}
			}
		}
	}

} //query_model
