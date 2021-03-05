#include "InA_Interpreter.h"

#include <InA_query_model/InA_query_model.h>
#include <query_generator/query_generator.h>
#include <dbproxy/dbproxy.h>
#include <metadata_enrichment/Tables.h>
#include <cube/cube.h>

#include "server_info_response.h"

#include <fstream>      // std::ifstream
#include <sstream>
#include <iostream>
#include <ios>
#include <iomanip>
#include <stdexcept>

#define WASM_EXPORT extern "C"


WASM_EXPORT
const char* json_getServerInfo()
{
	static std::string static_str_getserverinfo;
	if(static_str_getserverinfo.empty() )
	{
		std::ifstream ifs("../resources/response_getSerververInfo.json");
		if(ifs.is_open() )
			static_str_getserverinfo = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		else
			throw std::runtime_error("Could not open file../resources/response_getSerververInfo.json");
	}
	return static_str_getserverinfo.c_str();
}

WASM_EXPORT
const char* json_getResponse_json(const char* InA)
{
	// std::cout << "InA_Interpreter => getResponse call received: '" << InA << "'" << std::endl;
	
	JSONReader reader;
	JSONGenericObject root = reader.parse(InA);
	
	std::ostringstream osstream;
	JSONWriter writer(osstream);

	{
		JSON_MAP(writer);
		writer.key("message");

		const char* response = ina_interpreter::processRequest(root, writer);
		if(response != nullptr )
			return response;
	}
	static std::string static_str;
	static_str = osstream.str();
	
	return static_str.c_str();
}

namespace ina_interpreter
{
	using namespace wasabi;

	const char* processRequest(const JSONGenericObject& topElement, JSONWriter& writer)
	{
		// std::cout << "InA_Interpreter => processRequest" << std::endl;

		if(topElement.haveValue("Batch"))
		{
			// std::cout << "InA_Interpreter => Process 'Batch' InA request" << std::endl;
			const auto& batch = topElement.getArray("Batch");
			JSON_LIST(writer);
			for(int i = 0;i < batch.size();i++)
			{
				processRequest(batch[i], writer);
			}
		}
		if(const auto& metadata = topElement.getObject("Metadata"))
		{
			return processMetadataRequest(topElement.getObject("Metadata"));
		}
		if(const auto& analytics = topElement.getObject("Analytics"))
		{
			query_model::InA_query_model queryModel;
			parseAnalyticsRequest(topElement.getObject("Analytics"), queryModel);

			const query_generator::query_generator& queryGen = query_generator::query_generator(queryModel);
			
			const std::string sql = queryGen.getSQL();
			std::cout << "InA_Interpreter => Generated SQL: " << sql << std::endl;
			writer.value("SQL = " + sql);

			cube::Cube cube;
			queryGen.prepareCube(cube);

			if(!queryModel.getCnxString().empty() )
			{
				size_t line = 0;
				std::ostringstream results;
				std::function<void(const dbproxy::Row&)> lambda = [&line, &results, &cube](const dbproxy::Row& row)
				{
					cube.insertRow(row);
					for(int i = 0;i < row.size(); i++)
					{
						results << "  " << std::setw(10) << row[i].getString() << "  |  ";
					}
					results << std::endl;
					line++;
				};

				dbproxy::DBProxy::getDBProxy(queryModel.getCnxString())->executeSQL(sql, &lambda);
				std::cout << "InA_Interpreter => Results of SQL execution : " << std::endl  << results.str() << std::endl;
				writer.value("Results = " + results.str());
			}			
		}
		else
		{
			std::cerr << "InA_Interpreter => Unsupported InA request" << std::endl;
			writer.value("Error: unsupported InA request");
		}
		return nullptr;
	}

	void parseAnalyticsRequest(const JSONGenericObject& analytics, query_model::InA_query_model& queryModel)
	{
		// parse connectionString
		if(const auto& datasource = analytics.getObject("DataSource"))
		{
			if(datasource.haveValue("PackageName"))
				queryModel.setCnxString(datasource.getString("PackageName"));
			if(datasource.haveValue("ObjectName"))
				queryModel.setTable(datasource.getString("ObjectName"));
		}
		// parse dimensions/measures
		if(const auto& definition = analytics.getObject("Definition"))
		{
			if(const auto& dims = definition.getArray("Dimensions"))
			{
				for(int i = 0;i < dims.size();i++)
				{
					const auto& dim = dims[i];
					query_model::InA_dimension dimensionObj(dim.getString("Name"), dim.getString("Axis"));

					if(const auto& members = dim.getArray("Members"))
					{
						for(int i = 0;i < members.size();i++)
						{
							const auto& member = members[i];
							const std::string agg = member.haveValue("Aggregation")? member.getString("Aggregation") : "";
							const std::string name = member.getObject("MemberOperand")? member.getObject("MemberOperand").getString("Name") : member.getString("Name");
							dimensionObj.addMember(query_model::InA_member(name, agg));
						}
					}
					queryModel.addDimension(dimensionObj);
				}
			}
			if(const auto& subSelections = definition.getObject("DynamicFilter").getObject("Selection").getObject("Operator").getArray("SubSelections"))
			{
				for(size_t i = 0; i < subSelections.size(); ++i)
				{
					const std::string fieldName = subSelections[i].getObject("SetOperand").getString("FieldName");
					if(const auto& elements = subSelections[i].getObject("SetOperand").getArray("Elements"))
					{
						for(size_t i = 0; i < elements.size(); ++i)
						{	
							query_model::InA_queryFilterComparison queryFilterComparison(fieldName);
							std::string lowValue = elements[i].getString("Low");
							queryFilterComparison.setLowValue(lowValue);

							query_model::InA_queryFilter::ComparisonOperator comparisonOperator = query_model::InA_queryFilter::getComparator(elements[i].getString("Comparison"));
							queryFilterComparison.setComparisonOperator(comparisonOperator);
							queryModel.addQueryFilter(queryFilterComparison);
							std::cout << "InA_Interpreter => DynamicFilter -> ... -> Low :" << lowValue << std::endl;
						}
					}
				}
			}
		}

	}

	const char* processMetadataRequest(const JSONGenericObject& metadata)
	{
		std::string dataSourceName;
		if(const auto& datasource = metadata.getObject("DataSource"))
			if(datasource.haveValue("ObjectName"))
				dataSourceName = datasource.getString("ObjectName");
		std::string cnxString;
		if(const auto& packageName = metadata.getObject("PackageName"))
			if(packageName.haveValue("ObjectName"))
				cnxString = packageName.getString("PackageName");

		
		if(const JSONGenericObject& expand = metadata.getArray("Expand"))
		{
			bool expandCube = false;
			for(size_t i = 0; i < expand.size() ; i ++ )
			{
				if(expand.getString(i) == "Cubes")
				{
					expandCube = true;
					break;
				}
			}

			if(expandCube)
			{
				static std::string static_str_response;
				if(dataSourceName == "$$DataSource$$" )
				{
					if(static_str_response.empty() )
					{
						std::ifstream ifs("../resources/response_getResponse_Metadat_expand_cube_catalog.json");
						if(ifs.is_open() )
							static_str_response = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
						else
							throw std::runtime_error("Could not open file ../resources/response_getResponse_Metadat_expand_cube_catalog.json");
					}
					return static_str_response.c_str();
				}
				else
				{
					//TODO: Where will come from the cnx string in the InA
					std::shared_ptr<metadata::Catalog> catalog = std::shared_ptr<metadata::Catalog>(new metadata::Catalog("local:sqlite:efashion.db"));
					
					const auto& colNames = catalog->getTable(dataSourceName).getColumnNames();
					static_str_response += "|\t";
					for(const auto& colName : colNames)
						static_str_response += "\t|";
					return static_str_response.c_str();
				}
			}
		}

		throw std::runtime_error("Error: unsupported InA request");
	}
}
