#include "Attribute.h"

#include "Dimension.h"

#include <cstdint>
#include <json/jsonWriter.h>

namespace ina::metadata
{
	void writeDefaultExcludingOperators(JSONWriter &writer, const Attribute& att);
	void writeDefaultIncludingOperators(JSONWriter &writer, const Attribute& att);

	void write(const Attribute& att, JSONWriter& writer)
	{
		const Dimension& dim = att.getDimension();

		JSON_MAP(writer);
		writer.pair("Name", att.getName());
		writer.pair("Description", att.getDescription());
		writer.pair("NameExternal", att.getNameExternal());
		writer.pair("AliasName", att.getAlias());
		writer.pair("DimensionName", dim.getName());
		writer.pair("HasDescriptions", true);
		writer.pair("DescriptionColumn", att.getDescription());
		writer.pair("Cardinality", att.getCardinality());
		writer.pair("ColumnType", static_cast<std::uint32_t>(att.getColumnType()));
		writer.pair("DataType", toString(att.getDatatype()));
		writer.pair("DataTypeAsInt", static_cast<std::uint32_t>(att.getDatatype()));
		writer.pair("SQLType", att.getSQLDataType());


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
		writer.pair("IsKey", att.isKey());
		writer.pair("PresentationType", att.getPresentationType());
		writer.pair("IsMeasure", false);
		writer.pair("IsNotNull", att.isNotNull());
		writer.pair("IsVirtual", false);
		writer.pair("Level", 0u);
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

	void writeDefaultIncludingOperators(JSONWriter& writer, const Attribute& att)
	{
		const bool isFilterable = att.isFilterable();
		const Dimension& dim = att.getDimension();
		writer.key("Including");
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
				writer.value("BETWEEN");
				if (dim.getDimensionType() != eDimType::MeasuresDimension)
				{
					writer.value("NOT_BETWEEN");
					writer.value("LIKE");
					writer.value("MATCH");
				}
			}
		}
	}
}
