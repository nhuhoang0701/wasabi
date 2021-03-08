#include "InA_Interpreter.h"

#include <InA_query_model/Query.h>
#include <InA_query_model/DataSource.h>

#include <query_generator/query_generator.h>

#include <dbproxy/dbproxy.h>

#include <metadata_enrichment/Tables.h>

#include <cube/cube.h>


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
	std::cout << "InA_Interpreter => json_getServerInfo call received" << std::endl;
	static std::string static_str_getserverinfo;
	if(static_str_getserverinfo.empty() )
	{
		std::ifstream ifs("./resources/response_getSerververInfo.json");
		if(ifs.is_open() )
			static_str_getserverinfo = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		else
			throw std::runtime_error("Could not open file ./resources/response_getSerververInfo.json");
	}
	return static_str_getserverinfo.c_str();
}

WASM_EXPORT
const char* json_getResponse_json(const char* InA)
{
	std::cout << "InA_Interpreter => getResponse call received: '" << InA << "'" << std::endl;
	JSONReader reader;
	JSONGenericObject root = reader.parse(InA);
	
	std::cout << "InA_Interpreter => getResponse JSON to object model" << std::endl;
	std::vector<std::shared_ptr<ina::query_model::Query>> queries;
	read(queries, root);
	std::cout << "                   nb of queries: '" << queries.size() << "'" << std::endl;
	
	std::ostringstream osstream;
	JSONWriter writer(osstream);
	
	{
		JSON_MAP(writer);
		for(const auto& query : queries)
		{
				writer.key("message");

				const char* response = ina_interpreter::processRequest(*query, writer);
				if(response != nullptr )
					return response;
		}
	}
	static std::string static_str;
	static_str = osstream.str();
	
	return static_str.c_str();
}

namespace ina_interpreter
{
	using namespace wasabi;

	const char* processRequest(const ina::query_model::Query& query, JSONWriter& writer)
	{
		if(query.getType() == ina::query_model::Query::qMetadata)
		{
			if(query.haveExpandCube())
			{
				static std::string static_str_response;
				static_str_response.clear();
				if(query.getDataSource().getType() == ina::query_model::DataSource::Type::TypeCatalog )
				{
					if(static_str_response.empty() )
					{
						std::ifstream ifs("./resources/response_getResponse_Metadat_expand_cube_catalog.json");
						if(ifs.is_open() )
							static_str_response = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
						else
							throw std::runtime_error("Could not open file ./resources/response_getResponse_Metadat_expand_cube_catalog.json");
					}
					return static_str_response.c_str();
				}
				else
				{
					const std::string& cnxString = query.getDataSource().getPackageName();
					const std::string& tableName = query.getDataSource().getObjectName();

					std::shared_ptr<metadata::Catalog> catalog = std::shared_ptr<metadata::Catalog>(new metadata::Catalog(cnxString));
					
					const auto& colNames = catalog->getTable(tableName).getColumnNames();
					static_str_response += "|\t";
					for(const auto& colName : colNames)
						static_str_response += colName + "\t|";
					std::cout << static_str_response << std::endl;
					return static_str_response.c_str();
				}
			}
			
		}
		if(query.getType() == ina::query_model::Query::qAnalytics)
		{
			const query_generator::query_generator& queryGen = query_generator::query_generator(query);
			
			const std::string sql = queryGen.getSQL();
			std::cout << "InA_Interpreter => Generated SQL: " << sql << std::endl;
			writer.value("SQL = " + sql);

			cube::Cube cube;
			queryGen.prepareCube(cube);

			const std::string& cnxString = query.getDataSource().getPackageName();
			if(!cnxString.empty() )
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

				dbproxy::DBProxy::getDBProxy(cnxString)->executeSQL(sql, &lambda);
				std::cout << "InA_Interpreter => Results of SQL execution : " << std::endl  << results.str() << std::endl;
				writer.value("Results = " + results.str());
			}			
		}
		else
		{
			std::cerr << "InA_Interpreter => Unsupported InA request" << std::endl;
		}
		return nullptr;
	}
}
