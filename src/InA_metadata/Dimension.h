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

	enum class eAxe {Uninit=0, Rows, Columns};
	enum class eDimType {Uninit=0, MeasuresDimension = 2, Dimension = 3};

	class Dimension
	{
		public:

			Dimension() = default;
			Dimension(const std::string & name, eAxe axename);

			eAxe                getAxe() const;
			
			const std::string&  getName() const;
			const std::string&  getUniqueName() const;
			const std::string&  getNameExternal() const;
			const std::string&  getDescription() const;

			virtual eDimType             getDimensionType() const;
			virtual bool                 isDimensionGroup() const;
			virtual bool                 isModeled() const;
			virtual bool                 canBeAggregated() const;
			virtual const std::string&   getAxisDefault() const;

			void                          addAttribute(const Attribute &att);
			const std::vector<Attribute>& getAttributes() const;
			const Attribute&              getKeyAttribute() const;
			bool                          haveTextAttribute() const;
			const Attribute&              getTextAttribute() const;

			uint32_t                        getCardinality() const;

			void                          addMember(const Member & member);
			const std::vector<Member>&    getMembers() const;

		private:
			std::string _name; 
			eAxe        _axe = eAxe::Uninit;

			std::vector<Member>     _members;
			std::vector<Attribute>  _attributes;
			int8_t                  _keyAttributes = -1; // index in _attributes
			int8_t                  _textAttributes = -1;// index in _attributes

			friend void write(const Dimension& obj, JSONWriter& jsonNode);
	};

	class DimensionMeasures : public Dimension
	{
		public:
			DimensionMeasures() = default;
			DimensionMeasures(const std::string & name, eAxe axename);

			virtual eDimType             getDimensionType() const;
			virtual bool                 isDimensionGroup() const;
			virtual bool                 isModeled() const;
			virtual bool                 canBeAggregated() const;
			virtual const std::string&   getAxisDefault() const;
	};
}
