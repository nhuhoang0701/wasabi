#include "Member.h"

#include <json/jsonWriter.h>

namespace ina::metadata
{
	void write(const Member& member, JSONWriter& writer)
	{				
		JSON_MAP(writer);
		writer.pair(member.getUniqueName().first, member.getUniqueName().second);
		writer.pair(member.getName().first, member.getName().second);
		writer.pair(member.getDescription().first, member.getDescription().second);
	}
}
