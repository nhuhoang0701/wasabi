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
	// std::cout << "InA_Interpreter => getResponse call received: " << InA << std::endl;
	
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
			auto& batch = topElement.getArray("Batch");
			JSON_LIST(writer);
			for(int i = 0;i < batch.size();i++)
			{
				processRequest(batch[i], writer);
			}
		}
		else if(topElement.haveValue("Metadata"))
		{
			// std::cout << "InA_Interpreter => Process 'Metadata' InA request" << std::endl;
			writer.value("Process 'Metadata' InA request");
			
			static std::string static_str_response;
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
		else if(topElement.haveValue("Analytics"))
		{
			JSON_LIST(writer);
			// std::cout << "InA_Interpreter => Process 'Analytics' InA request" << std::endl;
			writer.value("Process 'Analytics' InA request");

			auto& analytics = topElement.getObject("Analytics");
			processAnalyticsRequest(analytics, writer);
		}
		else
		{
			std::cerr << "InA_Interpreter => Unsupported InA request" << std::endl;
			writer.value("Error: unsupported InA request");
		}
		return nullptr;
	}

	void processAnalyticsRequest(const JSONGenericObject& analytics, JSONWriter& writer)
	{
		query_model::InA_query_model queryModel;
		cube::Cube cube;

		std::shared_ptr<dbproxy::DBProxy> dbProxy;
		std::shared_ptr<Catalog> catalog;

		// parse connectionString
		if(const auto& datasource = analytics.getObject("DataSource"))
		{
			if(datasource.haveValue("ObjectName"))
			{
				queryModel.setTable(datasource.getString("ObjectName"));
			}
			if(const auto& props = datasource.getObject("CustomProperties"))
			{
				if(props.haveValue("cnxString"))
				{
					dbProxy = dbproxy::DBProxy::getDBProxy(props.getString("cnxString"));
					if(!dbProxy.get())
					{
						throw std::ios_base::failure("No database connection");
					}

					if(dbProxy)
						catalog = std::shared_ptr<Catalog>(new Catalog(*dbProxy));
				}
			}
		}
		// parse dimensions/measures
		if(const auto& definition = analytics.getObject("Definition"))
		{
			if(const auto& dims = definition.getArray("Dimensions"))
			{
				for(int i = 0;i < dims.size();i++)
				{
					const std::string& dimensionName = dims[i].getString("Name");
					// std::cout << "InA_Interpreter => requested dimension " << dim << std::endl;
					query_model::Datatype datatype = "";
					query_model::Aggregation aggregation = "";
					
					if(catalog)
					{
						const Table& table = catalog->getTable(queryModel.getTable());
						const auto& col = table.getColumn(dimensionName);
						datatype = query_model::InA_query_model::getModelDatatype(col.getDataType());
						aggregation = query_model::InA_query_model::getModelAggregation(col.getAggregation());
						if(aggregation.empty())
						{
							query_model::InA_dimension dimension(dimensionName, query_model::InA_dimension::Type::ObjectsDimension, datatype);
							queryModel.addDimension(dimension);
						}
						else
						{
							query_model::InA_dimension dimensionMeasure(dimensionName, query_model::InA_dimension::Type::MeasuresDimension, datatype);
							query_model::InA_member measure1(dimensionName, datatype, aggregation);
							queryModel.addDimension(dimensionMeasure);
						}
					}
				}
			}
			if(const auto& subSelections = definition.getObject("DynamicFilter").getObject("Selection").getObject("Operator").getArray("SubSelections"))
			{
				for(size_t i = 0; i < subSelections.size(); ++i)
				{
					if(const auto& elements = subSelections[i].getObject("SetOperand").getArray("Elements"))
					{
						for(size_t i = 0; i < elements.size(); ++i)
						{
							std::cout << "InA_Interpreter => DynamicFilter -> ... -> Low :" << elements[i].getString("Low") << std::endl;
						}
					}
				}
			}
		}

		std::cout << "InA_Interpreter => QueryModel: " << queryModel << std::endl;

		const query_generator::query_generator& queryGen = query_generator::query_generator(queryModel);
		const std::string sql = queryGen.getSQL();
		std::cout << "InA_Interpreter => Generated SQL: " << sql << std::endl;
		writer.value("SQL = " + sql);

		queryGen.prepareCube(cube);

		if(dbProxy)
		{
			const TableDescr& tableDescr = dbProxy->getTableDescr(queryModel.getTable());
			const std::vector<ColumnDescr>& cols = tableDescr.getColumnsDescr();
			size_t line = 0;
			std::ostringstream results;
			std::function<void(const Row&)> lambda = [&cols, &line, &results, &cube](const Row& row)
			{
				cube.insertRow(row);
				for(int i = 0;i < row.size(); i++)
				{
					results << "  " << std::setw(10) << row[i].getString() << "  |  ";
				}
				results << std::endl;
				line++;
			};
			dbProxy->executeSQL(sql, &lambda);
			std::cout << "InA_Interpreter => Results of SQL execution : " << std::endl  << results.str() << std::endl;
			writer.value("Results = " + results.str());
		}
	}
}
