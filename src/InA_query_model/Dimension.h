#pragma once

#include "Member.h"
#include "Attribute.h"

#include <string>
#include <vector>


class JSONGenericObject; // From <json/jsonReader.h>

namespace ina::query_model
{
	class Definition;
	class Dimension;

	void read(Dimension& obj, const JSONGenericObject& jsonNode);

	class Dimension
	{
		public:
			const static std::string DIMENSION_OF_MEASURES_NAME;

			inline static bool isDimensionOfMeasures(const Dimension & dimension)
			{
				return dimension.getName() == DIMENSION_OF_MEASURES_NAME;
			}

		public:
			enum class eAxe {Uninit=0, Rows, Columns};

			Dimension() = default;

			//TODO: Remove this constructor, ina::query_model::Dimension should only be calles from a InA json query
			Dimension(const std::string & name, eAxe axename);
			
			const std::string&  getName() const;
			eAxe                getAxe() const;

			const std::vector<Member>&    getMembers() const;

			const std::vector<Attribute>& getAttributes() const;
		
		//private:
		// Only for test units
			void                        addMember(const Member & member);
			void                        addAttribute(const Attribute &att);

		private:
			std::string _name; 
			eAxe        _axe = eAxe::Uninit;

			std::vector<Member>     _members;
			std::vector<Attribute>  _attributes;

			friend void read(Dimension& obj, const JSONGenericObject& jsonNode);			
			friend void read(Definition& obj, const JSONGenericObject& jsonNode);
	};
}