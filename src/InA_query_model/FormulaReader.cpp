#include "Formula.h"
#include "Function.h"

#include <json/jsonReader.h>    // For JSONGenericObject
#include <stdexcept>

namespace ina::query_model
{
	void read(Formula& formula, const JSONGenericObject& formulaNode)
	{
		read(formula.m_parameter, formulaNode);
	}
}
