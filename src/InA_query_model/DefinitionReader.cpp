#include "Definition.h"

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
				query_model::Dimension dimensionObj;
				read(dimensionObj, dims[i]);
				definition.addDimension(dimensionObj);
			}
		}
		if(const auto& subSelections = definitionNode.getObject("DynamicFilter").getObject("Selection").getObject("Operator").getArray("SubSelections"))
		{
			buildQueryFilter(definition, subSelections);
		}
	}

	void buildQueryFilter(Definition & definition, const JSONGenericObject & subSelections) 
	{
		bool isSubSelectionArray = false;
		for(size_t i = 0; i < subSelections.size(); ++i)
		{
			auto currentSubSelection = subSelections[i];
			while (!currentSubSelection.getObject("SetOperand") && !isSubSelectionArray)
			{
				if (const auto currentOperator = currentSubSelection.getObject("Operator"))				
				{
					if (currentOperator.getArray("SubSelections"))
					{
						if (currentOperator.getArray("SubSelections").size() == 1)
						{
							currentSubSelection = currentOperator.getArray("SubSelections")[0];
						}
						else
						{							
							currentSubSelection = currentOperator.getArray("SubSelections");
							isSubSelectionArray = true;
							buildQueryFilter(definition, currentSubSelection);
						}
					}
				}
			}

			if (!isSubSelectionArray)
			{
				const std::string fieldName = currentSubSelection.getObject("SetOperand").getString("FieldName");
				if(const auto& elements = currentSubSelection.getObject("SetOperand").getArray("Elements"))
				{
					for(size_t j = 0; j < elements.size(); ++j)
					{	
						query_model::InA_queryFilterComparison queryFilterComparison(fieldName);
						if (elements[j].haveValue("Low"))
						{
							std::string lowValue = elements[j].getString("Low");
							queryFilterComparison.setLowValue(lowValue);
						}
						
						std::string comparisonValue = elements[j].getString("Comparison");
						query_model::InA_queryFilter::ComparisonOperator comparisonOperator = query_model::InA_queryFilter::getComparisonOperator(comparisonValue);
						queryFilterComparison.setComparisonOperator(comparisonOperator);
						if (elements[j].haveValue("IsExcluding"))
						{
							bool isExcluding = elements[j].getBool("IsExcluding");
							queryFilterComparison.setExcluding(isExcluding);
						}
						definition.addQueryFilter(queryFilterComparison);
					}
				}
			}
		}
	}

} //query_model
