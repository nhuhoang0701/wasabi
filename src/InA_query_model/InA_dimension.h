#pragma once

#include "InA_member.h"

#include <string>
#include <vector>

class JSONGenericObject; // From <json/jsonReader.h>
class JSONWriter;        // From <json/jsonWriter.h>

namespace ina::query_model
{
	class Definition;

	class Dimension;
	void read(Dimension& obj, const JSONGenericObject& jsonNode);
	void write(const Dimension& obj, JSONWriter& jsonNode);

	class Dimension
	{
		public:
			Dimension(const std::string & name, const std::string & axename);
			
			const std::string & getName() const;
			const std::string & getAxeName() const;

			const std::vector<InA_member> & getMembers() const;
			void                            addMember(const InA_member & member);

		private:
			Dimension() = default;

			std::string _name; 
			std::string _axename; 

			std::vector<InA_member> _members;

			friend void read(Dimension& obj, const JSONGenericObject& jsonNode);
			friend void write(const Dimension& obj, JSONWriter& jsonNode);
			
			friend void read(Definition& obj, const JSONGenericObject& jsonNode);
	};
}