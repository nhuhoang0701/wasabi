#include "InA_dimension.h"

namespace ina::query_model
{
	static void writeAttributeHierarchy(JSONWriter& writer, const Dimension& dim);
	static void writeAttributeChildrens(JSONWriter& writer, const Dimension& dim);
	static void writeHierarchies(JSONWriter& writer, const Dimension& dim);
	static void writeAttributes(JSONWriter& writer, const Dimension& dim);
	static void writeMembers(JSONWriter& writer, const Dimension& dim);
	static void writeAttribute(JSONWriter& writer, const InA_Attribute& att);
	static void writeDefaultExcludingOperators(JSONWriter &writer, const InA_Attribute& att);
	static void writeDefaultIncludingOperators(JSONWriter &writer, const InA_Attribute& att);

	enum AttributeColumnType
	{
		NumericMeasure = 73,
		NoNumericMeasure = 83,
		NoMeasure = 0
	};
	static std::string  toString(AttributeColumnType);
	static const AttributeColumnType getColumnType(const InA_Attribute& att);

	void write(const Dimension& dim, JSONWriter& dimNode)
	{
		JSON_MAP(writer);

		writeAttributeHierarchy(writer, dim);

		writeAttributes(writer, dim);

		writeHierarchies(writer, dim);

		//
		const bool isMeasure = dim.getDimensionType() == Dimension::MeasuresDimension;
		const bool isVariable = dim.getDimensionType() == Dimension::VariableDimension;

		writer.key("AxisConstraints");
		{
			JSON_LIST(writer);
			{
				if (isVariable)
					writer.value("Technical");
				else
				{
					writer.value("Free");
					writer.value("Rows");
					writer.value("Columns");
				}
			}
		}
		if (isMeasure)
			writer.pair("AxisDefault", "Columns");
		else if (isVariable)
			writer.pair("AxisDefault", "Technical");
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

		writer.pair("Description", dim.getDescription());
		writer.pair("DimensionType", isMeasure ? dimTypeMeasure : dimTypeAttribute);
		writer.pair("KeyAttribute", dim.getKeyAttribute().getName());
		writer.pair("TextAttribute", "[Measures].[Description]");

		writeMembers(writer, dim);

		writer.pair("Name", dim.getName());
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

		writer.pair("UniqueName", dim.getName());
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

	void writeAttributeHierarchy(JSONWriter & writer, const Dimension & dim)
	{
		writer.key("AttributeHierarchy");
		{
			JSON_MAP(writer);
			writer.key("AttributeNames");
			{
				JSON_LIST(writer);
				for (std::vector<InA_Attribute>::const_iterator iter = dim.beginAttributes(); iter != dim.endAttributes(); ++iter)
				{
					writer.value(iter->getName());
				}
			}

			writeAttributeChildrens(writer, dim);

			switch (dim.getDimensionType())
			{
				case Dimension::MeasuresDimension:
				{
					const bo_utf8string& defaultKeyAttribute = dim.getAttribute(model::InA_Attribute::Key).getName();
					const bo_utf8string& defaultDisplayKeyAttribute = dim.getAttribute(model::InA_Attribute::Name).getName();
					const bo_utf8string& defaultTextAttribute = dim.getAttribute(model::InA_Attribute::Name).getName();

					writer.pair("DefaultDisplayKeyAttribute", defaultDisplayKeyAttribute);
					writer.pair("DefaultKeyAttribute", defaultKeyAttribute);
					writer.key("DefaultResultSetAttributes");
					{
						JSON_LIST(writer);
						writer.value(defaultDisplayKeyAttribute);
					}
					writer.pair("DefaultTextAttribute", defaultTextAttribute);
				}
				break;
				/*
				case Dimension::VariableDimension:
				{

					const std::string& defaultDisplayKeyAttribute = dim.getKeyAttribute().getDescription();
					const std::string& defaultKeyAttribute = dim.getKeyAttribute().getDescription();


					writer.pair("DefaultDisplayKeyAttribute", defaultDisplayKeyAttribute);
					writer.pair("DefaultKeyAttribute", defaultKeyAttribute);
					writer.key("DefaultResultSetAttributes");
					{
						JSON_LIST(writer);
						for (std::vector<InA_Attribute>::const_iterator iter = dim.beginAttributes(); iter != dim.endAttributes(); ++iter)
						{
							writer.value(iter->getDescription());
						}
					}
					if (dim.hasAttribute(InA_Attribute::Description))
					{
						const std::string defaultTextAttribute = reinterpret_cast<const char*>(dim.getAttribute(InA_Attribute::Description).getDescription().c_str());
						writer.pair("DefaultTextAttribute", defaultTextAttribute);
					}
				}
				break;
				*/
				case Dimension::ObjectsDimension:
				{
					const std::string& defaultKeyAttribute = dim.getAttribute(model::InA_Attribute::Key).getName();
					const std::string& defaultDisplayKeyAttribute = defaultKeyAttribute;
					const std::string& defaultTextAttribute = dim.getAttribute(model::InA_Attribute::Description).getName();

					writer.pair("DefaultDisplayKeyAttribute", defaultDisplayKeyAttribute);
					writer.pair("DefaultKeyAttribute", defaultKeyAttribute);
					writer.pair("DefaultTextAttribute", defaultTextAttribute);
					writer.key("DefaultResultSetAttributes");
					{
						JSON_LIST(writer);
						writer.value(defaultDisplayKeyAttribute);
					}

					/*
					if (dim.countHierarchies() > 0)
					{
						const std::string& hDefaultKeyAttribute  = dim.getAttribute(model::InA_Attribute::HKey).getName();
						const std::string& hDefaultTextAttribute = dim.getAttribute(model::InA_Attribute::HDescription).getName();
						const std::string& hDefaultPathAttribute = dim.getAttribute(model::InA_Attribute::HPath).getName();
						const std::string& hDefaultNameAttribute = dim.getAttribute(model::InA_Attribute::HName).getName();

						writer.pair("HierarchyDisplayKeyAttribute", hDefaultNameAttribute);
						writer.pair("HierarchyKeyAttribute", hDefaultKeyAttribute);
						writer.pair("HierarchyPathAttribute", hDefaultPathAttribute);
						writer.pair("HierarchyTextAttribute", hDefaultTextAttribute);
					}
					*/
				}
				break;

				default:
					throw TRACED_InA_EXCEPTION("Unexpected type of Dimension");
			}

			writer.pair("Description", dim.getDescription());
			writer.pair("Name", "Root"); // anything else breaks java test aggregation_calculatedMeasure()
		}
	}

	void writeAttributeChildrens(JSONWriter & writer, const Dimension & dim)
	{
		writer.key("Children");
		{
			JSON_LIST(writer);

			/*
			if (dim.getDimensionType() == Dimension::VariableDimension)
			{
				for (std::vector<InA_Attribute>::const_iterator iter = dim.beginAttributes(); iter != dim.endAttributes(); ++iter)
				{
					if (iter->getAttributeType() != InA_Attribute::Key && iter->getAttributeType() != InA_Attribute::Description)
					{
						JSON_MAP(writer);
						writer.key("AttributeNames");
						{
							JSON_LIST(writer);
							writer.value(iter->getDescription());
						}
						writer.pair("DefaultKeyAttribute", iter->getDescription());
						writer.key("DefaultResultSetAttributes");
						{
							JSON_LIST(writer);
							writer.value(iter->getDescription());
						}
						writer.pair("DefaultTextAttribute", "");
						writer.pair("Description", "");
						writer.pair("Name", iter->getAlias());
					}
				}
			}
			*/
		}
	}

	void writeHierarchies(JSONWriter & writer, const Dimension & dim)
	{
		writer.pair("NumberOfHierarchies", 0);
		writer.pair("HierarchiesPossible", false);
		/*
		writer.pair("NumberOfHierarchies", dim.countHierarchies());
		writer.pair("HierarchiesPossible", dim.countHierarchies()!=0);

		if (dim.countHierarchies() != 0)
		{
			if(dim.getDefaultHierarchy() != NULL)
				writer.pair("DefaultHierarchy", dim.getDefaultHierarchy()->getName() );

			writer.key("Hierarchies");
			JSON_LIST(writer);
			for (std::vector<InA_Hierarchy>::const_iterator it = dim.beginHierarchies();
				it != dim.endHierarchies(); ++it)
			{
				InA_HierarchyBuilder::write(writer, *it);
			}
		}
		*/
	}
	void writeMembers(JSONWriter & writer, const Dimension & dim)
	{
		if (dim.countMembers() > 0 && dim.getDimensionType() != Dimension::MeasuresDimension)
			throw TRACED_InA_EXCEPTION("Members are expected in dimension of measures only");

		writer.key("Members");
		{
			JSON_LIST(writer);

			for (std::vector<Member>::const_iterator it = dim.beginMembers(); it != dim.endMembers(); ++it)
			{
				const Member& member = *it;

				JSON_MAP(writer);
				writer.pair("[Measures].[Measures]", member.getName());
				writer.pair("[Measures].[Name]", member.getDescription()); // ? bizarre ?
				writer.pair("[Measures].[Description]", member.getDescription());
				writer.pair("[Measures].[Aggregation]", member.getModelAggregation());
				writer.pair("[Measures].[SQLType]", member.getSqlDataType());
				writer.pair("[Measures].[SemanticType]", member.getSemanticType());
			}
		}
	}

	void InA_DimensionBuilder::writeAttributes(JSONWriter & writer, const Dimension & dim)
	{
		writer.key("Attributes");
		JSON_LIST(writer);

		for (std::vector<InA_Attribute>::const_iterator iter = dim.beginAttributes(); iter != dim.endAttributes(); ++iter)
		{
			writeAttribute(writer, *iter);
		}
	}

	void InA_DimensionBuilder::writeAttribute(JSONWriter& writer, const InA_Attribute& att)
	{
		const Dimension& dim = att.getDimension();

		const bool isMeasure = dim.getDimensionType() == Dimension::MeasuresDimension;
		const bool isVariable = dim.getDimensionType() == Dimension::VariableDimension;

		const std::string& dataType = att.getDataTypeAsString();
		const AttributeColumnType columnType = getColumnType(att);
		const int dataTypeAsInt = att.getDataTypeAsInt();


		JSON_MAP(writer);
		writer.pair("AliasName", att.getAlias());
		writer.pair("Cardinality", isMeasure ? 0 : DefaultAttributeCardinality);
		writer.pair("ColumnType", columnType);
		writer.pair("DataType", dataType);
		writer.pair("DataTypeAsInt", dataTypeAsInt);

		writer.pair("Description", att.getDescription());

		writer.pair("DescriptionColumn", att.getDescription());
		writer.pair("Digits", 0);
		writer.pair("DimensionName", dim.getName());
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
		writer.pair("HasDescriptions", dim.getDimensionType() == Dimension::MeasuresDimension || dim.getDimensionType() == Dimension::VariableDimension || dim.getDimensionType() == Dimension::ObjectsDimension);
		writer.pair("HasFulltextIndex", false);
		writer.pair("IsDefaultText", dim.getDimensionType() == Dimension::VariableDimension);
		writer.pair("IsDisplayAttribute", att.isDisplayed());
		writer.pair("IsFilterable", att.isFilterable());
		writer.pair("IsFreestyle", false);
		writer.pair("IsKey", att.isFlatKey());
		writer.pair("IsMeasure", false);
		writer.pair("IsNotNull", !isMeasure);
		writer.pair("IsVirtual", false);
		writer.pair("Level", 0);
		writer.pair("Name", att.getName());

		writer.pair("PresentationType", model::toString(att.getPresentationType()));

		if (isMeasure)
			writer.pair("SQLType", columnType == NumericMeasure ? "INTEGER" : "VARCHAR");

		if (!isMeasure)
			writer.pair("UpperBound", "");// no present for measure
	}

	void InA_DimensionBuilder::writeDefaultExcludingOperators(JSONWriter& writer, const InA_Attribute& att)
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

	void InA_DimensionBuilder::writeDefaultIncludingOperators(JSONWriter& writer, const InA_Attribute& att)
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

	std::string InA_DimensionBuilder::toString(AttributeColumnType type)
	{
		switch (type)
		{
		case NumericMeasure: return "NumericMeasure";
		case NoNumericMeasure: return "NoNumerciMeasure";
		case NoMeasure: return "NoMeasure";
		default:  throw TRACED_InA_EXCEPTION("Unknown Attribute column type"); break;
		}
	}

	const InA_DimensionBuilder::AttributeColumnType InA_DimensionBuilder::getColumnType(const InA_Attribute & att)
	{
		const Dimension& dim = att.getDimension();
		if (dim.getDimensionType() == Dimension::MeasuresDimension)
		{
			if (att.getDataType() == InA_Types::Long || att.getDataType() == InA_Types::Double)
				return InA_DimensionBuilder::NumericMeasure;
			return InA_DimensionBuilder::NoNumericMeasure;
		}
		return InA_DimensionBuilder::NoMeasure;
	}
}
