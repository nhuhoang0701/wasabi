#pragma once

#include <metadata_enrichment/Tables.h>

#include "InA_DataSource.h"
#include "InA_member.h"
#include "InA_dimension.h"
#include "InA_queryFilter.h"
#include "InA_queryFilterComparison.h"

#include <string>
#include <vector>
#include <ostream>


namespace grid {class Grid;}


namespace ina::query_model
{
	using namespace wasabi;

	typedef std::string Datatype;
	typedef std::string Aggregation;

	const Aggregation NO_AGG = "";

	class Definition;
	void read(Definition & model, const JSONGenericObject& definition);

	class Definition
	{
	public:
		Definition() {};
		
		void addDimension(const InA_dimension & dimension);

		const std::vector<InA_dimension>& getDimensions() const { return m_objs;};
		const std::vector<InA_member>& getVisibleMembers(const InA_dimension& dimension, std::vector<InA_member> & visibleMembers) const;

		const std::vector<InA_queryFilterComparison>& getQueryFilters() const { return m_filters;};
		void addQueryFilter(const InA_queryFilterComparison & queryFilterComparison);

		void prepareGrid(grid::Grid& grid);
		
		inline static Datatype getModelDatatype(const metadata::Column::DataType& datatype)
		{
			switch(datatype)
			{
				case 1:
					return "String";
					break;
				case 2:
					return "Numeric";
					break;
				case 3:
					return "DateTime";
					break;
			}
		}

		inline static Aggregation getModelAggregation(const metadata::Column::Aggregation& aggregation)
		{
			switch(aggregation)
			{
				case 0:
					return "";
					break;
				case 1:
					return "Sum";
					break;
				case 2:
					return "Max";
					break;
				case 3:
					return "Min";
					break;
				case 4:
					return "Count";
					break;
			}
		}

	private:
		std::vector<InA_dimension> m_objs;
		std::vector<InA_queryFilterComparison> m_filters;
		std::string m_table;
		std::string m_cnxString;
		
		DataSource m_ds;
		
		friend void read(Definition & model, const JSONGenericObject& definition);
	};

} // query_model