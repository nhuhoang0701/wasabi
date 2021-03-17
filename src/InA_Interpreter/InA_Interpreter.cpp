#include "InA_Interpreter.h"

#include <calculator/storage.h>
#include <calculator/cube.h>

#include <InA_query_model/Query.h>
#include <InA_query_model/DataSource.h>

#include <query_generator/query_generator.h>

#include <dbproxy/dbproxy.h>

#include <metadata_enrichment/Tables.h>


#include <memory>
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
	std::vector<std::shared_ptr<const ina::query_model::Query>> queries;
	read(queries, root);
	std::cout << "                   nb of queries: '" << queries.size() << "'" << std::endl;
	
	std::ostringstream osstream;
	JSONWriter writer(osstream);
	
	{
		JSON_MAP(writer);
		writer.key("message");
		for(const auto& query : queries)
		{
				JSON_LIST(writer);
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
					
					std::ostringstream results;

					const auto& colNames = catalog->getTable(tableName).getColumnNames();

					results << "Object name" << std::endl;
					for(auto& colName : colNames)
						results << "  " << std::setw(10) << catalog->getTable(tableName).getColumn(colName).getName() << "  |  ";
					results << std::endl;

					results << "SQL Name" << std::endl;
					for(auto& colName : colNames)
						results << "  " << std::setw(10) << catalog->getTable(tableName).getColumn(colName).getSQLName() << "  |  ";
					results << std::endl;

					results << "Datatype" << std::endl;
					for(auto& colName : colNames)
						results << "  " << std::setw(10) << catalog->getTable(tableName).getColumn(colName).getDataType() << "  |  ";
					results << std::endl;

					results << "Agg" << std::endl;
					for(auto& colName : colNames)
						results << "  " << std::setw(10) << catalog->getTable(tableName).getColumn(colName).getAggregation() << "  |  ";
					results << std::endl;

					static_str_response = results.str();
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

			const std::string& cnxString = query.getDataSource().getPackageName();
			if(!cnxString.empty() )
			{
				std::shared_ptr<calculator::DataStorage> data(new calculator::DataStorage());

				queryGen.prepareStorage(*data);
				std::function<void(const dbproxy::Row&)> lambda = [&data](const dbproxy::Row& row)
				{
					data->insertRow(row);
				};
				dbproxy::DBProxy::getDBProxy(cnxString)->executeSQL(sql, &lambda);

				std::ostringstream results;
				for(size_t idxCol = 0; idxCol < data->getColNbrs(); idxCol++)
				{
					results << "  " << std::setfill('-') << std::setw(10) << "" << "  |  ";
				}
				results << std::setfill(' ') << std::endl;

				for(size_t rowNb = 0; rowNb < data->getRowNbrs(); rowNb++)
				{
					for(size_t idxCol = 0; idxCol < data->getColNbrs(); idxCol++)
					{
						const auto col = (*data)[idxCol];
						const auto& colData = std::get<2>(col);
						const auto& colTyped = (*colData)[rowNb];
						switch (std::get<0>(col))
						{
						case calculator::eDataType::String:
							results << "  " << std::setw(10) << std::get<std::string>(colTyped) << "  |  ";
							break;
						case calculator::eDataType::Number:
							results << "  " << std::setw(10) << std::get<double>(colTyped) << "  |  ";
							break;
						}
					}
					results << std::endl;
				}

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
