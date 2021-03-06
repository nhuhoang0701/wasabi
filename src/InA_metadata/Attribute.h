#pragma once

#include <common/data.h>

#include <cstdint>
#include <string>
#include <string_view>

class JSONWriter;        // From <json/jsonWriter.h>

namespace ina::metadata
{
	class Dimension;
	class Attribute;

	void write(const Attribute& obj, JSONWriter& jsonNode);

	enum class eAttrType {Uninit=-1, Key = 3, Text = 1};
	enum class eColumnType {Uninit=-1,NumericMeasure = 73,NoNumericMeasure = 83,NoMeasure = 0};

	std::string_view   toString(common::eDataType);

	class Attribute
	{
		public:
			Attribute() = default;
			Attribute(const Dimension& dimension, const std::string name, const std::string& description, eAttrType type, common::eDataType datatype);
			Attribute(const Attribute& other);
			Attribute& operator=(const Attribute& other);

			const Dimension& getDimension() const;

			const std::string&  getName() const;
			const std::string&  getUniqueName() const;
			const std::string   getNameExternal() const;
			const std::string   getDescription() const;

			const std::string&   getAlias() const;

			common::eDataType    getDataType() const;
			const std::string&   getSQLDataType() const;
			
			bool                 isKey() const;
			const std::string    getPresentationType() const;

			eColumnType          getColumnType() const;

			bool                 isDisplayed() const;
			bool                 isFilterable() const;

			bool                 isNotNull() const;

			std::uint32_t        getCardinality() const;

		private:
			std::string       m_name; 
			std::string       m_description;
			common::eDataType m_datatype = common::eDataType::Undefined;
			eAttrType         m_type = eAttrType::Uninit;

			const Dimension* m_dimension = nullptr;

			friend void write(const Attribute& obj, JSONWriter& jsonNode);
	};
}
