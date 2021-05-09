#include "Cube.h"
#include "Dimension.h"
#include "Attribute.h"
#include "InA_metadata/Member.h"

#include <InA_query_model/DataSource.h>

#include <json/jsonWriter.h>
#include <stdexcept>
#include <vector>

namespace ina::metadata
{
	void writeDimensions(const Cube& cube, JSONWriter& writer)
	{
		writer.key("Dimensions");
		{
			JSON_LIST(writer);
			for(const auto& dim : cube.getDimensions())
				write(*dim, writer);
		}
	}

	void writeQuery(const Cube& cube, JSONWriter &writer)
	{
		writer.key("Query");
		{
			JSON_MAP(writer);
			writer.key("ResultStructureFeature");
			{
				JSON_MAP(writer);
				writer.pair("ConditionalTotals", false);
				writer.pair("ConditionalVisibility", false);
				writer.pair("Enabled", true);
				writer.key("ResultAlignment");
				{
					JSON_MAP(writer);
					writer.pair("ConfigLevel", "Dimension");
					writer.key("ResultAlignmentList");
					{
						JSON_LIST(writer);
						writer.value("Top");
						writer.value("Bottom");
						writer.value("TopBottom");
					}
				}
				writer.key("ReturnedDataSelection");
				{
					JSON_MAP(writer);
					writer.pair("Actions", false);
					writer.pair("CellDataType", false);
					writer.pair("CellFormat", false);
					writer.pair("CellMeasure", false);
					writer.pair("CellValueTypes", true);
					writer.pair("ExceptionAlertLevel", false);
					writer.pair("ExceptionName", false);
					writer.pair("ExceptionSettings", false);
					writer.pair("Exceptions", false);
					writer.pair("InputEnabled", false);
					writer.pair("NumericRounding", false);
					writer.pair("NumericShift", false);
					writer.pair("TupleDisplayLevel", false);
					writer.pair("TupleDrillState", false);
					writer.pair("TupleElementIds", false);
					writer.pair("TupleElementIndexes", false);
					writer.pair("TupleLevel", false);
					writer.pair("TupleParentIndexes", false);
					writer.pair("UnitDescriptions", false);
					writer.pair("UnitTypes", false);
					writer.pair("Units", false);
					writer.pair("Values", true);
					writer.pair("ValuesFormatted", true);
					writer.pair("ValuesRounded", false);
				}
			}
		}

	}

	void write(const Cube& cube, JSONWriter& writer)
	{
		writer.key("Cube");
		{
			JSON_MAP(writer);
			writer.pair("Description", "");
			writer.pair("CatalogPackage", "");
			writer.pair("CatalogSchema", "");
			writer.pair("CatalogType", "");
			writer.pair("CatalogView", "");
			writer.pair("CreatedBy", "");
			writer.pair("CreatedOn", "");
			write(cube.m_datasource, writer);
			writeDimensions(cube, writer);
			writer.key("ExtendedSortTypes");
			{
				JSON_LIST(writer);
				writer.value("MemberKey");
				writer.value("MemberText");
				writer.value("Field");
				writer.value("Measure");
				writer.value("Complex");
				writer.value("Datacell");
			}
			writer.pair("InputEnabled", false);
			writer.pair("InstanceId", "");
			writer.pair("IsSystem", false);
			writer.pair("LastDataUpdateBy", "ghislain.hude@sap.com");
			writer.pair("LastDataUpdateOn", "2021-05-07 22:22:22.022");
			writer.pair("LevelOfDetails", 1u);
			writer.pair("ObjectName", "eFashion-Simple2.unx");
			writer.pair("PackageName", "Universes/InA Provider/Test Resources/AggregationTest");
			writer.pair("SchemaName", "");
			writeQuery(cube, writer);
			writer.key("Variables");{JSON_LIST(writer);}
			writer.key("SortTypesBreakGrouping");
			{
				JSON_LIST(writer);
				writer.value("MemberKey");
				writer.value("MemberText");
				writer.value("Field");
				writer.value("Measure");
				writer.value("Complex");
				writer.value("Datacell");
			}
		}
	}
}
