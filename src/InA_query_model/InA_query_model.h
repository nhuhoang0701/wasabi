#pragma once

#include <metadata_enrichment/Tables.h>

#include <string>
#include <vector>
#include <ostream>


class JSONWriter;
class JSONGenericObject;
namespace grid {class Grid;}

using namespace wasabi::metadata;

namespace query_model
{
	enum {eUninitialize = -1, eDim = 1, eMeas = 2} eQualification;
	typedef std::string Datatype;
	typedef std::string Aggregation;
	typedef std::tuple<const std::string /*name*/, const Datatype, const Aggregation > Object;

	const Aggregation NO_AGG = "";

	class InA_query_model
	{
	public:
		InA_query_model() {};
		
		InA_query_model(const JSONGenericObject& serializedQueryModel);
		
		void setTable(const std::string& table) {m_table = table;};

		void addDim(const std::string& name, const Datatype& datatype);

		void addMeas(const std::string& name, const Datatype& datatype, const Aggregation& agg);

		const std::vector<Object>& getObjects() const { return m_objs;};

		const std::string& getTable() const { return m_table;};

		void prepareGrid(grid::Grid& grid);
		
      	void write(JSONWriter& writer) const;
		
		inline static Datatype getModelDatatype(const Column::DataType& datatype)
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

		inline static Aggregation getModelAggregation(const Column::Aggregation& aggregation)
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
		std::vector<Object> m_objs;
		std::string m_table;
	};
	
    std::ostream& operator<<(std::ostream& os, const InA_query_model& queryModel);
} // query_model
