#include "Attribute.h"

#include <json/jsonWriter.h>

namespace ina::metadata
{
	static void writeDefaultExcludingOperators(JSONWriter &writer, const Attribute& att);
	static void writeDefaultIncludingOperators(JSONWriter &writer, const Attribute& att);

	enum AttributeColumnType
	{
		NumericMeasure = 73,
		NoNumericMeasure = 83,
		NoMeasure = 0
	};
	static std::string               toString(AttributeColumnType);
	static const AttributeColumnType getColumnType(const Attribute& att);

	void write(const Attribute& dim, JSONWriter& writer)
	{
		const Dimension& dim = att.getDimension();

		const bool isMeasure = dim.getDimensionType() == Dimension::MeasuresDimension;

		const std::string& dataType = att.getDataTypeAsString();
		const AttributeColumnType columnType = getColumnType(att);
		const int dataTypeAsInt = att.getDataTypeAsInt();


		JSON_MAP(writer);
		writer.pair("Name", att.getName());
		writer.pair("Description", att.getDescription());
		writer.pair("AliasName", att.getAlias());
		writer.pair("DimensionName", dim.getName());
		writer.pair("HasDescriptions", true);
		writer.pair("DescriptionColumn", att.getDescription());
		writer.pair("Cardinality", isMeasure ? 0 : DefaultAttributeCardinality);
		writer.pair("ColumnType", columnType);
		writer.pair("DataType", dataType);
		writer.pair("DataTypeAsInt", dataTypeAsInt);


		writer.pair("Digits", 0);
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
		writer.pair("FractDigits", 0);
		writer.pair("HasFulltextIndex", false);
		writer.pair("IsDefaultText", false);
		writer.pair("IsDisplayAttribute", att.isDisplayed());
		writer.pair("IsFilterable", att.isFilterable());
		writer.pair("IsFreestyle", false);
		writer.pair("IsKey", att.isFlatKey());
		writer.pair("IsMeasure", false);
		writer.pair("IsNotNull", !isMeasure);
		writer.pair("IsVirtual", false);
		writer.pair("Level", 0);

		writer.pair("PresentationType", model::toString(att.getPresentationType()));

		if (isMeasure)
			writer.pair("SQLType", columnType == NumericMeasure ? "INTEGER" : "VARCHAR");

		if (!isMeasure)
			writer.pair("UpperBound", "");// no present for measure
	}

	void writeDefaultExcludingOperators(JSONWriter& writer, const Attribute& att)
	{
		//const bool isFilterable = att.isFilterable();
		//const Dimension& dim = att.getDimension();
		writer.key("Excluding");
		{
			JSON_LIST(writer);
			/*if (isFilterable)
			{
				writer.value("=");
				writer.value(">");
				writer.value("<");
				writer.value(">=");
				writer.value("<=");
				writer.value("<>");
				if (dim.getDimensionType() != Dimension::MeasuresDimension)
				{
					writer.value("BETWEEN");
					writer.value("NOT_BETWEEN");
					writer.value("LIKE");
					writer.value("MATCH");
				}
			}*/
		}
	}

	void writeDefaultIncludingOperators(JSONWriter& writer, const Attribute& att)
	{
		//const bool isFilterable = att.isFilterable();
		//const Dimension& dim = att.getDimension();
		writer.key("Including");
		{
			JSON_LIST(writer);
			/*if (isFilterable)
			{
				writer.value("=");
				writer.value(">");
				writer.value("<");
				writer.value(">=");
				writer.value("<=");
				writer.value("<>");
				writer.value("BETWEEN");
				if (dim.getDimensionType() != Dimension::MeasuresDimension)
				{
					writer.value("NOT_BETWEEN");
					writer.value("LIKE");
					writer.value("MATCH");
				}
			}*/
		}
	}
}
