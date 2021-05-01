#pragma once

#include <metadata_enrichment/Tables.h>

#include "DataSource.h"
#include "InA_query_model/QuerySort.h"
#include "Member.h"
#include "Dimension.h"
#include "QueryFilter.h"
#include "QueryFilterComparison.h"

#include <string>
#include <vector>
#include <ostream>

class JSONGenericObject;    // #include <json/jsonReader.h>

namespace ina::query_model
{
	using namespace wasabi;

	typedef std::string Datatype;
	typedef std::string Aggregation;

	const Aggregation NO_AGG = "";

	class Definition;
	void read(Definition& obj, const JSONGenericObject& jsonNode);
	void buildQueryFilter(Definition& obj, const JSONGenericObject& jsonNode);

	class Definition
	{
	public:
		Definition() = default;
		
		void addDimension(const Dimension & dimension);

		const std::vector<Dimension>& getDimensions() const;

		const std::vector<Member>    getVisibleMembers(const Dimension& dimension) const;

		const std::vector<QueryFilterComparison>& getQueryFilters() const;
		void                                          addQueryFilter(const QueryFilterComparison & queryFilterComparison);

		const std::vector<QuerySort>& getQuerySorts() const;
		void addQuerySort(const QuerySort& querySort);
		
		inline static Datatype getModelDatatype(const metadata::Column::DataType& datatype)
		{
			switch(datatype)
			{
				case metadata::Column::DataType::String:
					return "String";
					break;
				case metadata::Column::DataType::Numeric:
					return "Numeric";
					break;
				case metadata::Column::DataType::DateTime:
					return "DateTime";
					break;
			}
		}

		inline static Aggregation getModelAggregation(const metadata::Column::Aggregation& aggregation)
		{
			switch(aggregation)
			{
				case metadata::Column::Aggregation::None :
					return "";
					break;
				case metadata::Column::Aggregation::Sum:
					return "Sum";
					break;
				case metadata::Column::Aggregation::Max:
					return "Max";
					break;
				case metadata::Column::Aggregation::Min:
					return "Min";
					break;
				case metadata::Column::Aggregation::Count:
					return "Count";
					break;
			}
		}

	private:
		std::vector<Dimension> m_objs;
		std::vector<QueryFilterComparison> m_filters;
		std::vector<QuerySort> m_sorts;
		std::string m_table;
		std::string m_cnxString;
		
		DataSource m_ds;
		
		friend void read(Definition & obj, const JSONGenericObject& jsonNode);
	};

} // query_model
