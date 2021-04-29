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
		if(queries.empty() == false)
			write(queries[0]->getDataSource(), writer);
		writer.key("Grids");
		{
			JSON_LIST(writer);
			for(const auto& query : queries)
			{
				JSON_MAP(writer);
				const char* response = ina_interpreter::processRequest(*query, writer);
				if(response != nullptr )
					return response;
			}
		}
	}

	static std::string static_str;
	static_str = osstream.str();
	
	return static_str.c_str();
}

void writeDimensions(JSONWriter& writer, std::vector<const ina::query_model::Dimension*>& dims, const calculator::Cube& cube)
{
	JSON_LIST(writer);
	size_t idxDim = 0;
	for(const auto& dim : dims)
	{
		JSON_MAP(writer);
		writer.pair("Name", dim->getName());
		{
			writer.key("Attributes");
			JSON_LIST(writer);
			{
				JSON_MAP(writer);
				writer.pair("Name", dim->getName());
				writer.key("Values");
				{
					JSON_LIST(writer);
					if(!ina::query_model::Dimension::isDimensionOfMeasures(*dim) )
					{
						const auto& col = *cube.getStorage().getColumn(dim->getName());
						//TODO: false should dump the dimension value
						for(size_t rowIdx = 0; rowIdx < col.getNbDistinctVals(); rowIdx++)
						{
							const auto& data = col.getDistinctValue(rowIdx);
							switch (col.getDataType())
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
							default:
								throw std::runtime_error("InA_Interpreter => Unsupported datatype.");
							}
						}
					}
					else
					{
						for(const auto& member : dim->getMembers())
							writer.value(member.getName());
					}
				}
			}
		}
		idxDim++;
	}
}

void writeTuples(JSONWriter &writer, calculator::Axe &axis, size_t &axisIndex) 
{
	for (size_t rowIndex = 0; rowIndex < axis.getCardinality(); rowIndex++) 
	{
		auto valueDataType = axis.getValueDatatype(axisIndex);
		auto value = axis.getValue(axisIndex, rowIndex);
		switch (valueDataType) 
		{
			case calculator::eDataType::String: 
			{
				writer.value(std::get<std::string>(value));
				break;
			}
			case calculator::eDataType::Number: 
			{
				writer.value(std::get<double>(value));
				break;
			}
			default:
				throw std::runtime_error("InA_Interpreter => Unsupported datatype.");
		}
	}
}

void writeTuples(JSONWriter& writer, const ina::query_model::Definition & definition , const calculator::Cube& cube)
{
	JSON_LIST(writer);
	{
		auto rowAxis 		= cube.getAxe(calculator::Cube::eAxe::Row);
		size_t rowAxisIndex = 0;

		auto columnAxis 		= cube.getAxe(calculator::Cube::eAxe::Column);
		size_t columnAxisIndex 	= 0;

		for(auto dimension  : definition.getDimensions())
		{
			if(ina::query_model::Dimension::isDimensionOfMeasures(dimension) )
			{
				// Done in Cells
				continue;
			}
			JSON_MAP(writer);
			{
				writer.key("TupleElementIds");
				JSON_MAP(writer);
				{
					writer.pair("Encoding","None");
					writer.key("Values");
					JSON_LIST(writer);
					{
						if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
							writeTuples(writer, rowAxis, rowAxisIndex);
						else if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
							writeTuples(writer, columnAxis, columnAxisIndex);
						else
							throw std::runtime_error("InA_Interpreter => Unknow axis type.");
					}
				}
			}
		}
	}
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
					if(query.getDataSource().getObjectName() == "$$DataSource$$" )
					{
						static std::string static_str_response;
						static_str_response.clear();
						if(static_str_response.empty() )
						{
							std::ifstream ifs("./resources/response_getResponse_Metadata_expand_cube_catalog.json");
							if(ifs.is_open() )
								static_str_response = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
							else
								throw std::runtime_error("Could not open file ./resources/response_getResponse_Metadata_expand_cube_catalog.json");
						}
						return static_str_response.c_str();
					}
					else if(query.getDataSource().getObjectName() == "Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma" )
					{
						static std::string static_str_response;
						static_str_response.clear();
						if(static_str_response.empty() )
						{
							std::ifstream ifs("./resources/response_getResponse_Metadata_expand_cube.json");
							if(ifs.is_open() )
								static_str_response = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
							else
								throw std::runtime_error("Could not open file ./resources/response_getResponse_Metadata_expand_cube.json");
						}
						return static_str_response.c_str();
					}
					else
					{
						const std::string& cnxString = query.getDataSource().getPackageName();
						const std::string& tableName = query.getDataSource().getObjectName();

						std::shared_ptr<metadata::Catalog> catalog = std::shared_ptr<metadata::Catalog>(new metadata::Catalog(cnxString));
						
						{
							const auto& colNames = catalog->getTable(tableName).getColumnNames();
							{
								writer.key("Object name");
								JSON_LIST(writer);
								for(auto& colName : colNames)
									writer.value(catalog->getTable(tableName).getColumn(colName).getName());
							}
							{
								writer.key("Object SQL name");
								JSON_LIST(writer);
								for(auto& colName : colNames)
									writer.value(catalog->getTable(tableName).getColumn(colName).getSQLName());
							}
							{
								writer.key("Datatype");
								JSON_LIST(writer);
								for(auto& colName : colNames)
									writer.value(catalog->getTable(tableName).getColumn(colName).getDataType());
							}
							{
								writer.key("Agg");
								JSON_LIST(writer);
								for(auto& colName : colNames)
									writer.value(catalog->getTable(tableName).getColumn(colName).getAggregation());
							}
						}
					}
				}
				else
					throw std::runtime_error("InA_Interpreter => Unsupported InA request");
				break;
			}
			case (ina::query_model::Query::qAnalytics):
			{
				const ina::query_model::Dimension* measDim = nullptr; // TODO: Workaround waiting to have a grid
				std::vector<const ina::query_model::Dimension*> rowDims; // TODO: Workaround waiting to have a grid
				std::vector<const ina::query_model::Dimension*> colDims; // TODO: Workaround waiting to have a grid
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
						if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
							rowDims.push_back(&dimension);
						else if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
							colDims.push_back(&dimension);

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
							measDim = &dimension;
							for(const auto& member : dimension.getMembers())
								cube.addMeas(member.getName());
						}
					}
				}
				cube.materialyze();
				{
					writer.key("Axes");
					{
						JSON_LIST(writer);
						if(!rowDims.empty())
						{
							JSON_MAP(writer);
							writer.pair("Type", "Rows");
							writer.pair("Name", "ROWS");
							writer.key("Dimensions");
							writeDimensions(writer, rowDims, cube);
						}
						if(!colDims.empty())
						{
							JSON_MAP(writer);
							writer.pair("Type", "Columns");
							writer.pair("Name", "COLUMNS");
							writer.key("Dimensions");
							writeDimensions(writer, colDims, cube);
						}
					}
					writer.key("Tuples");
					{
						writeTuples(writer, query.getDefinition(), cube);
					}
					
					
					writer.key("CellArraySizes");
					{
						JSON_LIST(writer);
						writer.value(cube.getBody().getColNbrs() * (measDim && measDim->getAxe()==ina::query_model::Dimension::eAxe::Columns? measDim->getMembers().size():1 ));
						writer.value(cube.getBody().getRowNbrs() * (measDim && measDim->getAxe()==ina::query_model::Dimension::eAxe::Rows? measDim->getMembers().size():1 ));
					}
					writer.key("Cells");
					{
						JSON_MAP(writer);
						writer.key("Values");
						{
							JSON_MAP(writer);
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
											default:
												throw std::runtime_error("InA_Interpreter => Unsupported datatype.");
											}
										}
									}
								}
							}
						}
					}
					writer.pair("ResultSetState", 0);
					writer.pair("HasErrors", false);
				}

				break;
			}
			default:
			{
				throw std::runtime_error("InA_Interpreter => Unsupported InA request");
			}
		}
		return nullptr;
	}
}
