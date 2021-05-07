#pragma once

#include <string>

class JSONWriter;        // From <json/jsonWriter.h>

namespace ina::metadata
{
	class Dimension;
	class Attribute;

	void write(const Attribute& obj, JSONWriter& jsonNode);

	enum class eDatatype {Uninit=-1, String = 2, Number = 3};
	enum class ColumnType {Uninit=-1};

	class Attribute
	{
		public:

			Attribute(const Dimension& dimension);
			
			const std::string&  getName() const;
			const std::string&  getUniqueName() const;
			const std::string&  getNameExternal() const;
			const std::string&  getDescription() const;

			eDatatype             getDatatype() const;
			virtual bool                 isDimensionGroup();
			virtual bool                 isModeled() const;
			virtual bool                 canBeAggregated() const;
			virtual const std::string&   getAxisDefault() const;

			size_t                        getCardinality() const;

		private:
			std::string _name; 

			friend void write(const Attribute& obj, JSONWriter& jsonNode);
	};
}
