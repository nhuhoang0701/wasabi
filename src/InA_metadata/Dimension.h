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

	enum class eAxe {Uninit=0, Free, Rows, Columns};
	std::string toString(eAxe);
	enum class eDimType {Uninit=0, MeasuresDimension = 2, Dimension = 3};

	class Dimension
	{
		public:
			Dimension() = default;
			Dimension(const std::string& name, const std::string& description, eAxe defaultAxe = eAxe::Free);
			
			const std::string&  getName() const;
			const std::string&  getUniqueName() const;
			const std::string   getNameExternal() const;
			const std::string   getDescription() const;

			eAxe     getAxisDefault() const {return m_defaultaxe;};

			virtual eDimType             getDimensionType() const {return eDimType::Dimension;};
			virtual bool                 isDimensionGroup() const {return false;};
			virtual bool                 isModeled() const {return true;};
			virtual bool                 canBeAggregated() const {return true;};

			void                          addAttribute(const Attribute &att);
			const std::vector<Attribute>& getAttributes() const;
			const Attribute&              getKeyAttribute() const;
			bool                          haveTextAttribute() const;
			const Attribute&              getTextAttribute() const;

			virtual uint32_t              getCardinality() const {return 10000;};

			void                          addMember(const Member & member);
			const std::vector<Member>&    getMembers() const;

		protected:
			std::string m_name; 
			std::string m_description; 
			eAxe        m_defaultaxe = eAxe::Free;

			std::vector<Member>     m_members;
			std::vector<Attribute>  m_attributes;
			int8_t                  m_keyAttributes = -1; // index in m_attributes
			int8_t                  m_textAttributes = -1;// index in m_attributes

			friend void write(const Dimension& obj, JSONWriter& jsonNode);
	};

	class DimensionMeasures : public Dimension
	{
		public:
			DimensionMeasures() = default;
			DimensionMeasures(const std::string& name, const std::string& description, eAxe defaultAxe = eAxe::Free);

			virtual uint32_t              getCardinality() const {return m_members.size();};

			virtual eDimType             getDimensionType() const {return eDimType::MeasuresDimension;};
			virtual bool                 isDimensionGroup() const {return true;};
			//virtual bool                 isModeled() const;
			virtual bool                 canBeAggregated() const {return false;};
	};
}
