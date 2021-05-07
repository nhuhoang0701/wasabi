#pragma once

#include <string>

class JSONWriter;        // From <json/jsonWriter.h>

namespace ina::metadata
{
	class Dimension;
	class Member;

	void write(const Member& obj, JSONWriter& jsonNode);
	class Member
	{
		public:

			Member(const Dimension& dimension, const std::string& name, const std::string& value);
			
			const std::pair<std::string, std::string>&  getName() const;
			const std::pair<std::string, std::string>&  getUniqueName() const;
			const std::pair<std::string, std::string>&  getDescription() const;

		private:
			friend void write(const Member& obj, JSONWriter& jsonNode);
	};
}
