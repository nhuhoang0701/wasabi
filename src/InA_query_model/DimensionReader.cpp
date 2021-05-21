#include "Dimension.h"

#include <json/jsonReader.h>    // For JSONGenericObject
#include <stdexcept>

namespace ina::query_model
{
	void read(Dimension& dim, const JSONGenericObject& dimNode)
	{
		dim._name = dimNode.getString("Name");
		const std::string axisName = dimNode.getString("Axis");
		if(axisName=="Rows")
			dim._axe = Dimension::eAxe::Rows;
		else if(axisName=="Columns")
			dim._axe = Dimension::eAxe::Columns;
		else
			throw std::runtime_error("Unkown InA Axis: " + axisName);

		if(const auto& members = dimNode.getArray("Members"))
		{
			dim._members.resize(members.size());
			for(int i = 0;i < members.size();i++)
			{
				read(dim._members[i], members[i]);
			}
		}

		if(const auto& attributs = dimNode.getArray("Attributes"))
		{
			dim._attributes.resize(attributs.size());
			for(int i = 0;i < attributs.size();i++)
			{
				read(dim._attributes[i], attributs[i]);
			}
		}
	}
}
