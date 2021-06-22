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
			enum class eAxe {Uninit=0, None, Rows, Columns};

			Dimension() = default;

			//Only for test units
			Dimension(const std::string & name, eAxe axename);
			
			const std::string&  getName() const;
			eAxe                getAxe() const;

			const std::vector<Member>&    getMembers() const;

			bool                          haveAttribute(const std::string& name) const;
			const Attribute&              getAttribute(const std::string& name) const;
			const std::vector<Attribute>& getAttributes() const;
		
		//private:
		// Not private Only for test units
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