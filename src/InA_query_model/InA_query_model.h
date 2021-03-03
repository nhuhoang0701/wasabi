#pragma once

#include <metadata_enrichment/Tables.h>

#include "InA_member.h"
#include "InA_dimension.h"

#include <string>
#include <vector>
#include <ostream>


namespace grid {class Grid;}


namespace query_model
{
	using namespace wasabi;

	typedef std::string Datatype;
	typedef std::string Aggregation;

	const Aggregation NO_AGG = "";

	class InA_query_model
	{
	public:
		InA_query_model() {};
		
		void               setTable(const std::string& table) {m_table = table;};
		const std::string& getTable() const { return m_table;};
		
		void               setCnxString(const std::string& cnxString) {m_table = cnxString;};
		const std::string& getCnxString() const { return m_cnxString;};

		void addDimension(const InA_dimension & dimension);

		const std::vector<InA_dimension>& getDimensions() const { return m_objs;};


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
		std::string m_table;
		std::string m_cnxString;
	};

} // query_model
