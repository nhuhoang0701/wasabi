#include "Attribute.h"

#include "Dimension.h"

#include <cstdint>
#include <json/jsonWriter.h>

namespace ina::metadata
{
	static void writeDefaultExcludingOperators(JSONWriter &writer, const Attribute& att);
	static void writeDefaultIncludingOperators(JSONWriter &writer, const Attribute& att);

	void write(const Attribute& att, JSONWriter& writer)
	{
		const Dimension& dim = att.getDimension();

		//const std::string& dataType = att.getDataTypeAsString();
		//const uint32_t dataTypeAsInt = att.getDataTypeAsInt();
		//const uint32_t dataTypeSQL = att.getDataTypeAsInt();


		JSON_MAP(writer);
		writer.pair("Name", att.getName());
		writer.pair("Description", att.getDescription());
		writer.pair("AliasName", att.getAlias());
		writer.pair("DimensionName", dim.getName());
		writer.pair("HasDescriptions", true);
		writer.pair("DescriptionColumn", att.getDescription());
		writer.pair("Cardinality", att.getCardinality());
		writer.pair("ColumnType", static_cast<std::uint32_t>(att.getColumnType()));
		//writer.pair("DataType", dataType);
		//writer.pair("DataTypeAsInt", dataTypeAsInt);


		writer.pair("Digits", 0u);
		writer.key("FilterCapability");
		{
			JSON_MAP(writer);
			writer.key("Comparison");
			{
				JSON_MAP(writer);
				writeDefaultExcludingOperators(writer, att);
				writeDefaultIncludingOperators(writer, att);
			}
		}
		writer.pair("FractDigits", 0u);
		writer.pair("HasFulltextIndex", false);
		writer.pair("IsDefaultText", false);
		writer.pair("IsDisplayAttribute", att.isDisplayed());
		writer.pair("IsFilterable", att.isFilterable());
		writer.pair("IsFreestyle", false);
		writer.pair("IsKey", att.isFlatKey());
		writer.pair("IsMeasure", false);
		writer.pair("IsNotNull", att.isNotNull());
		writer.pair("IsVirtual", false);
		writer.pair("Level", 0u);

		//writer.pair("PresentationType", model::toString(att.getPresentationType()));

		//if (isMeasure)
		//	writer.pair("SQLType", att.getColumnType() == eColumnType::NumericMeasure ? "INTEGER" : "VARCHAR");

		//if (!isMeasure)
		//	writer.pair("UpperBound", "");// no present for measure
	}

	void writeDefaultExcludingOperators(JSONWriter& writer, const Attribute& att)
	{
		const bool isFilterable = att.isFilterable();
		const Dimension& dim = att.getDimension();
		writer.key("Excluding");
		{
			JSON_LIST(writer);
			if (isFilterable)
			{
				writer.value("=");
				writer.value(">");
				writer.value("<");
				writer.value(">=");
				writer.value("<=");
				writer.value("<>");
				if (dim.getDimensionType() != eDimType::MeasuresDimension)
				{
					writer.value("BETWEEN");
					writer.value("NOT_BETWEEN");
					writer.value("LIKE");
					writer.value("MATCH");
				}
			}
		}
	}
}
