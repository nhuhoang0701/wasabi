#include "Dimension.h"

#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{
	void read(Dimension& dim, const JSONGenericObject& dimNode)
	{
		dim._name = dimNode.getString("Name");
		dim._axename = dimNode.getString("Axis");

		if(const auto& members = dimNode.getArray("Members"))
		{
			for(int i = 0;i < members.size();i++)
			{
				query_model::Member member;
				read(member, members[i]);
				dim.addMember(member);
			}
		}

		if(const auto& attributs = dimNode.getArray("Attributes"))
		{
			for(int i = 0;i < attributs.size();i++)
			{
				Attribute attribut;
				read(attribut, attributs[i]);
				attribut.m_dim = &dim;
				dim.addAttribute(attribut);
			}
		}
	}
}
