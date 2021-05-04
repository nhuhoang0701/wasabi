#include "Dimension.h"

#include <json/jsonWriter.h>

namespace ina::query_model
{
	static void writeAttributeHierarchy(JSONWriter& writer, const Dimension& dim);
	static void writeAttributeChildrens(JSONWriter& writer, const Dimension& dim);
	static void writeHierarchies(JSONWriter& writer, const Dimension& dim);
	static void writeAttributes(JSONWriter& writer, const Dimension& dim);
	static void writeMembers(JSONWriter& writer, const Dimension& dim);
	static void writeAttribute(JSONWriter& writer, const Attribute& att);
	static void writeDefaultExcludingOperators(JSONWriter &writer, const Attribute& att);
	static void writeDefaultIncludingOperators(JSONWriter &writer, const Attribute& att);

	enum AttributeColumnType
	{
		NumericMeasure = 73,
		NoNumericMeasure = 83,
		NoMeasure = 0
	};
	static std::string  toString(AttributeColumnType);
	static const AttributeColumnType getColumnType(const Attribute& att);

	void write(const Dimension& dim, JSONWriter& writer)
	{
		const bool isMeasure = dim.getDimensionType() == Dimension::MeasuresDimension;

		JSON_MAP(writer);
		writer.pair("Name", dim.getName());
		writer.pair("UniqueName", dim.getName());
		writer.pair("NameExternal", dim.getName() + "_E");
		writer.pair("Description", dim.getName() + "_D");
		writer.pair("DimensionType", isMeasure ? dimTypeMeasure : dimTypeAttribute);

		writeAttributes(writer, dim);

		writer.pair("NumberOfHierarchies", 0);
		writer.pair("HierarchiesPossible", false);

		writer.key("AxisConstraints");
		{
			JSON_LIST(writer);
			writer.value("Free");
			writer.value("Rows");
			writer.value("Columns");
		}
		if (isMeasure)
			writer.pair("AxisDefault", "Columns");
		else
			writer.pair("AxisDefault", "Free");

		writer.pair("Cardinality", dim.countMembers()!=0 ? dim.countMembers() : DefaultAttributeCardinality);
		writer.key("DefaultResultStructure");
		{
			JSON_MAP(writer);

			writer.pair("ResultAlignment", "Bottom");
			writer.key("ResultStructure");
			{
				JSON_LIST(writer);
				{
					JSON_MAP(writer);
					writer.pair("Result", "Members");
					writer.pair("Visibility", "Visible");
				}
			}
		}

		writeMembers(writer, dim);

		writer.pair("Private", false);
		writer.pair("ResultSetFixedAttributes", false);
		writer.pair("ResultSetMemberDefaultReadMode", "Master");
		writer.pair("ResultSetMemberReadMode", "Master");

		writer.key("SupportedResultSetMemberReadModes");
		{
			JSON_LIST(writer);
			writer.value("Master");
			writer.value("Booked");
		}

		writer.pair("IsModeled", dim.isModeled());
		writer.key("DefaultResultSetAttributeNodes");
		{
			JSON_LIST(writer);
			writer.value("Root");
		}
		if (!isMeasure)
		{
			writer.key("DimensionGroups");
			{ JSON_LIST(writer); }
		}
		writer.pair("IsDimensionGroup", isMeasure);
		writer.pair("CanBeAggregated", !isMeasure);
	}

	void writeMembers(JSONWriter & writer, const Dimension & dim)
	{
		writer.key("Members");
		{
			JSON_LIST(writer);

			for (std::vector<Member>::const_iterator it = dim.beginMembers(); it != dim.endMembers(); ++it)
			{
				const Member& member = *it;

				JSON_MAP(writer);
				writer.pair("[Measures].[Measures]", member.getName());
				writer.pair("[Measures].[Name]", member.getDescription());
				writer.pair("[Measures].[Description]", member.getDescription());
			}
		}
	}

	void writeAttributes(JSONWriter & writer, const Dimension & dim)
	{
		writer.pair("KeyAttribute", dim.getKeyAttribute().getName());
		writer.pair("TextAttribute", dim.getTextAttribute().getName());
		writer.key("Attributes");
		JSON_LIST(writer);

		for (std::vector<Attribute>::const_iterator iter = dim.beginAttributes(); iter != dim.endAttributes(); ++iter)
		{
			writeAttribute(writer, *iter);
		}
	}

	void writeAttribute(JSONWriter& writer, const Attribute& att)
	{
		const Dimension& dim = att.getDimension();

		const bool isMeasure = dim.getDimensionType() == Dimension::MeasuresDimension;
		const bool isVariable = dim.getDimensionType() == Dimension::VariableDimension;

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
