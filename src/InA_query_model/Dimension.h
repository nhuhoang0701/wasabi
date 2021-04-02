#pragma once

#include "Member.h"
#include "Attribute.h"

#include <string>
#include <vector>



class JSONGenericObject; // From <json/jsonReader.h>
class JSONWriter;        // From <json/jsonWriter.h>

namespace ina::query_model
{
	const std::string DIMENSION_OF_MEASURES_NAME = "CustomDimension1";

	class Definition;

	class Dimension;

	void read(Dimension& obj, const JSONGenericObject& jsonNode);
	void write(const Dimension& obj, JSONWriter& jsonNode);

	class Dimension
	{
		public:
			enum class eAxe {Uninit=0, Rows, Columns};

			Dimension() = default;

			//TODO: Remove this constructor obj should only be createa from a InA json
			Dimension(const std::string & name, eAxe axename);
			
			const std::string & getName() const;
			eAxe                getAxe() const;

			const std::vector<Member> & getMembers() const;
			void                        addMember(const Member & member);

			const std::vector<Attribute>& getAttributes() const;
			void                          addAttribute(const Attribute &att);

		private:
			std::string _name; 
			eAxe        _axe = eAxe::Uninit;

			std::vector<Member>     _members;
			std::vector<Attribute>  _attributes;

			friend void read(Dimension& obj, const JSONGenericObject& jsonNode);
			friend void write(const Dimension& obj, JSONWriter& jsonNode);
			
			friend void read(Definition& obj, const JSONGenericObject& jsonNode);


		public:
			inline static bool isDimensionOfMeasures(const Dimension & dimension)
			{
				return dimension.getName() == DIMENSION_OF_MEASURES_NAME;
			}
	};
}