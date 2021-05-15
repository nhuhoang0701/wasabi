#include "Attribute.h"

#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{
	void read(Attribute& obj, const JSONGenericObject& jsonNode)
	{
		obj.m_name = jsonNode.getString("Name");
		if(jsonNode.haveValue("Obtainability"))//Not sure if this is mandatory
			obj.m_Obtainability = jsonNode.getString("Obtainability");
	}
}