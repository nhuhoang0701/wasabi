#pragma once

#include "InA_member.h"

#include <string>
#include <vector>

class JSONGenericObject; // From <json/jsonReader.h>
class JSONWriter;        // From <json/jsonWriter.h>

namespace ina::query_model
{
	class Definition;

	class InA_dimension;
	void read(InA_dimension& obj, const JSONGenericObject& jsonNode);
	void write(const InA_dimension& obj, JSONWriter& jsonNode);

	class InA_dimension
	{
		public:
			InA_dimension(const std::string & name, const std::string & axename);
			
			const std::string & getName() const;
			const std::string & getAxeName() const;

			const std::vector<InA_member> & getMembers() const;
			void                            addMember(const InA_member & member);

		private:
			InA_dimension() = default;

			std::string _name; 
			std::string _axename; 

			std::vector<InA_member> _members;

			friend void read(InA_dimension& obj, const JSONGenericObject& jsonNode);
			friend void write(const InA_dimension& obj, JSONWriter& jsonNode);
			
			friend void read(Definition& obj, const JSONGenericObject& jsonNode);
	};
}