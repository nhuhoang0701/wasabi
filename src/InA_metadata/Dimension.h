#pragma once

#include "Member.h"
#include "Attribute.h"

#include <string>
#include <vector>

class JSONWriter;        // From <json/jsonWriter.h>

namespace ina::metadata
{
	class Definition;
	class Dimension;

	void write(const Dimension& obj, JSONWriter& jsonNode);

	class Dimension
	{
		public:
			enum class eAxe {Uninit=0, Rows, Columns};
			enum class eDimType {Uninit=0, MeasuresDimension = 2, Dimension = 3};

			Dimension() = default;
			Dimension(const std::string & name, eAxe axename);

			eAxe                getAxe() const;
			
			const std::string&  getName() const;
			const std::string&  getUniqueName() const;
			const std::string&  getNameExternal() const;
			const std::string&  getDescription() const;

			eDimType             getDimensionType() const;
			bool                 isDimensionGroup();
			bool                 isModeled() const;
			bool                 canBeAggregated() const;
			const std::string&   getAxisDefault() const;

			void                          addAttribute(const Attribute &att);
			const std::vector<Attribute>& getAttributes() const;
			const Attribute&              getKeyAttribute() const;
			bool                          haveTextAttribute() const;
			const Attribute&              getTextAttribute() const;
		

			size_t              getCardinality() const;

			void                          addMember(const Member & member);
			const std::vector<Member>&    getMembers() const;

		private:
			std::string _name; 
			eAxe        _axe = eAxe::Uninit;

			std::vector<Member>     _members;
			std::vector<Attribute>  _attributes;

			friend void write(const Dimension& obj, JSONWriter& jsonNode);
	};
}
