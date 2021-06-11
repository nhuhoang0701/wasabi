#include "Definition.h"

#include "Selection/Selection.h"
#include "Sort.h"

#include "ResultSetFeatureRequest.h"
#include "common/Log.h"

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
		
		if(const auto& resSetFeatJSon = definitionNode.getObject("ResultSetFeatureRequest"))
		{
			Read(definition.m_resultSetFeature, resSetFeatJSon);
		}
		if (const auto& dynamicFilterNode = definitionNode.getObject("DynamicFilter"))
		{
			ScopeDebug sc("Reading DynamicFilter");
			if (const auto& selectionNode = dynamicFilterNode.getObject("Selection"))
			{
				read(definition.m_selection, selectionNode);
			}
		}

		if (const auto& sortArray = definitionNode.getArray("Sort"))
		{
			for(int i=0; i < sortArray.size(); i++)
			{
				query_model::Sort querySort;
				read(querySort, sortArray[i]);
				definition.addSort(querySort);
			}
		}
	}
} //query_model
