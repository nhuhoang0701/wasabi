#include "Dimension.h"

#include <cstdint>
#include <json/jsonWriter.h>

namespace ina::metadata
{
	static void writeAttributeHierarchy(JSONWriter& writer, const Dimension& dim);
	static void writeAttributes(JSONWriter& writer, const Dimension& dim);
	static void writeMembers(JSONWriter& writer, const Dimension& dim);

	void write(const Dimension& dim, JSONWriter& writer)
	{
		const bool isMeasure = dim.getDimensionType() == eDimType::MeasuresDimension;

		JSON_MAP(writer);
		writer.pair("Name", dim.getName());
		writer.pair("UniqueName", dim.getUniqueName());
		writer.pair("NameExternal", dim.getNameExternal());
		writer.pair("Description", dim.getDescription());
		writer.pair("DimensionType", static_cast<std::uint32_t>(dim.getDimensionType()));

		writeAttributeHierarchy(writer, dim);
		writeAttributes(writer, dim);

		writer.pair("NumberOfHierarchies", 0u);
		writer.pair("HierarchiesPossible", false);

		writer.key("AxisConstraints");
		{
			JSON_LIST(writer);
			if(dim.isAxisSupported(eAxe::Free))
				writer.value("Free");
			if(dim.isAxisSupported(eAxe::Rows))
				writer.value("Rows");
			if(dim.isAxisSupported(eAxe::Columns))
				writer.value("Columns");
		}
		writer.pair("AxisDefault", toString(dim.getAxisDefault()));

		writer.pair("Cardinality", dim.getCardinality());
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

		writer.key("DefaultResultSetAttributeNodes");
		{
			JSON_LIST(writer);
			writer.value("Root");
		}
		writer.pair("IsModeled", dim.isModeled());
		writer.pair("CanBeAggregated", dim.canBeAggregated());

		writer.pair("IsDimensionGroup", dim.isDimensionGroup());
		if (!isMeasure)
		{
			writer.key("DimensionGroups");
			{ JSON_LIST(writer); }
		}
	}

	void writeMembers(JSONWriter & writer, const Dimension & dim)
	{
		writer.key("Members");
		{
			JSON_LIST(writer);
			for (const auto& member : dim.getMembers())
			{
				write(member, writer);
			}
		}
	}

	void writeAttributeHierarchy(JSONWriter& writer, const Dimension& dim)
	{
		writer.key("AttributeHierarchy");
		{
			JSON_MAP(writer);
			writer.pair("Name", dim.getName());
			writer.pair("Description", dim.getDescription());
			writer.pair("DefaultKeyAttribute", dim.getKeyAttribute().getName());
			if(dim.haveTextAttribute())
				writer.pair("DefaultTextAttribute", dim.getTextAttribute().getName());
		}

	}

	void writeAttributes(JSONWriter & writer, const Dimension & dim)
	{
		writer.pair("KeyAttribute", dim.getKeyAttribute().getName());
		if(dim.haveTextAttribute())
			writer.pair("TextAttribute", dim.getTextAttribute().getName());
		writer.key("Attributes");
		{
			JSON_LIST(writer);
			for (const auto& attribute : dim.getAttributes())
			{
				write(attribute, writer);
			}
		}
	}
}
