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
	for(const auto& query : queries)
	{
		const char* response = ina_interpreter::processRequest(*query, writer);
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

	const char* processRequest(const ina::query_model::Query& query, JSONWriter& writer)
	{
		switch(query.getType())
		{
			case ina::query_model::Query::qMetadata :
			{
				if(query.haveExpandCube())
				{
					static std::string static_str_response;
					static_str_response.clear();
					if(query.getDataSource().getType() == ina::query_model::DataSource::Type::TypeCatalogView 
					  || query.getDataSource().getType() == ina::query_model::DataSource::Type::TypeView )
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
				break;
			}
			case (ina::query_model::Query::qAnalytics):
			{
				calculator::Cube cube;
				{
					const query_generator::query_generator& queryGen = query_generator::query_generator(query);

					std::shared_ptr<calculator::DataStorage> data(new calculator::DataStorage());
					queryGen.prepareStorage(*data);
					std::function<void(const dbproxy::Row&)> lambda = [&data](const dbproxy::Row& row)
					{
						data->insertRow(row);
					};
					const std::string& cnxString = query.getDataSource().getPackageName();
					dbproxy::DBProxy::getDBProxy(cnxString)->executeSQL(queryGen.getSQL(), &lambda);

					cube.setStorage(data);
					for (const auto& dimension : query.getDefinition().getDimensions()) 
					{
						if(!ina::query_model::Dimension::isDimensionOfMeasures(dimension) )
						{
							if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
								cube.addDim(calculator::Cube::eAxe::Row, dimension.getName());
							else if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
								cube.addDim(calculator::Cube::eAxe::Column, dimension.getName());
							else throw std::runtime_error("Unknow axis type");
						}
						else
						{
							for(const auto& member : dimension.getMembers())
								cube.addMeas(member.getName());
						}
					}
				}
				cube.materialyze();
				{
					JSON_MAP(writer);
					writer.key("Cells");
					{
						JSON_MAP(writer);
						writer.key("Values");
						{
							writer.pair("Encoding", "None");
							writer.key("Values");
							{
								JSON_LIST(writer);	
								for(size_t colIndex = 0; colIndex < cube.getBody().getColNbrs(); colIndex++)
								{
									for(size_t rowIndex = 0; rowIndex < cube.getBody().getRowNbrs(); rowIndex++)
									{
										for(size_t measIndex = 0; measIndex < cube.getBody().size(); measIndex++)
										{
											const auto& data = cube.getBody().getValue(measIndex, colIndex, rowIndex);
											switch (cube.getBody().getValueDatatype(measIndex))
											{
											case calculator::eDataType::String:
											{
												writer.value(std::get<std::string>(data));
												break;
											}
											case calculator::eDataType::Number:
											{
												writer.value(std::get<double>(data));
												break;
											}
											}
										}
									}
								}
							}
						}
					}
				}

				break;
			}
			default:
			{
				std::cerr << "InA_Interpreter => Unsupported InA request" << std::endl;
			}
		}
		return nullptr;
	}
}