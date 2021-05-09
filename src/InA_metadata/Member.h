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

			Member(const Dimension& dimension, const std::string& uniqueName, const std::string& name, const std::string& description);
			
			Member(const Member& other);
			Member& operator=(const Member& other);

			const std::pair<std::string, std::string>&  getName() const;
			const std::pair<std::string, std::string>&  getUniqueName() const;
			const std::pair<std::string, std::string>&  getDescription() const;

		private:
			std::pair<std::string, std::string> m_uniquename;
			std::pair<std::string, std::string> m_name;
			std::pair<std::string, std::string> m_description;
			friend void write(const Member& obj, JSONWriter& jsonNode);
	};
}
