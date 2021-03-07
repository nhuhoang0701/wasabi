#include "InA_dimension.h"

#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{
	void read(InA_dimension& dim, const JSONGenericObject& dimNode)
	{
		dim._name = dimNode.getString("Name");
		dim._axename = dimNode.getString("Axis");

		if(const auto& members = dimNode.getArray("Members"))
		{
			for(int i = 0;i < members.size();i++)
			{
				query_model::InA_member member;
				read(member, members[i]);
				dim.addMember(member);
			}
		}
	}
}
