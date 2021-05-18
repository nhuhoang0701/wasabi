#include "Formula.h"
#include "Function.h"

#include <json/jsonReader.h>    // For JSONGenericObject
#include <stdexcept>

namespace ina::query_model
{
	void read(Formula& formula, const JSONGenericObject& formulaNode)
	{
		formula.m_name = formulaNode.getString("Name");
		if(JSONGenericObject functionObj = formulaNode.getObject("Function"))
		{
			query_model::Function function;
			read(function, functionObj);
			formula.m_function = function;
		}
	}
}
