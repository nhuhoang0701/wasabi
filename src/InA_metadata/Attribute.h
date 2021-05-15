#pragma once

#include <cstdint>
#include <string>

class JSONWriter;        // From <json/jsonWriter.h>

namespace ina::metadata
{
	class Dimension;
	class Attribute;

	void write(const Attribute& obj, JSONWriter& jsonNode);

	enum class eDatatype {Uninit=-1, String = 3, Long = 1};
	std::string toString(eDatatype);
	enum class eColumnType {Uninit=-1,NumericMeasure = 73,NoNumericMeasure = 83,NoMeasure = 0};

	class Attribute
	{
		public:
			Attribute() = default;
			Attribute(const Dimension& dimension, const std::string name, const std::string& description);
			Attribute(const Attribute& other);
			Attribute& operator=(const Attribute& other);

			const Dimension& getDimension() const;

			const std::string&  getName() const;
			const std::string&  getUniqueName() const;
			const std::string   getNameExternal() const;
			const std::string   getDescription() const;

			const std::string&   getAlias() const;

			eDatatype            getDataType() const;
			const std::string&   getSQLDataType() const;
			
			bool                 isKey() const;
			const std::string    getPresentationType() const;

			eColumnType          getColumnType() const;

			bool                 isDisplayed() const;
			bool                 isFilterable() const;

			bool                 isNotNull() const;

			std::uint32_t        getCardinality() const;

		private:
			std::string      m_name; 
			std::string      m_description;

			const Dimension* m_dimension = nullptr;

			friend void write(const Attribute& obj, JSONWriter& jsonNode);
	};
}
