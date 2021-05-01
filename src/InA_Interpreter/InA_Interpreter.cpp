#include "InA_Interpreter.h"

#include <json/jsonReader.h>
#include <json/jsonWriter.h>

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
	//std::cout << "InA_Interpreter => json_getServerInfo call received" << std::endl;
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
	//std::cout << "InA_Interpreter => getResponse call received: '" << InA << "'" << std::endl;
	JSONReader reader;
	JSONGenericObject root = reader.parse(InA);
	
	//std::cout << "InA_Interpreter => getResponse JSON to object model" << std::endl;
	std::vector<std::shared_ptr<const ina::query_model::Query>> queries;
	read(queries, root);
	//std::cout << "                   nb of queries: '" << queries.size() << "'" << std::endl;
	
	std::ostringstream osstream;
	JSONWriter writer(osstream);
	{
		JSON_MAP(writer);
		if(queries.empty() == false)
		{
			write(queries[0]->getDataSource(), writer);
			{
				writer.key("Grids");
				JSON_LIST(writer);
				for(const auto& query : queries)
				{
					if(query->getType() == ina::query_model::Query::qAnalytics)
					{
						JSON_MAP(writer);
						ina::grid::writeGrid(*query, writer);
					}
					else if(query->getType() == ina::query_model::Query::qMetadata)
					{
						JSON_MAP(writer);
						const char* response = ina::grid::writeCube(*query, writer);
						if(response != nullptr )
							return response;
					}
					else
						throw std::runtime_error("InA_Interpreter => Unsupported InA request (should Metadata and Analytics)");
				}
			}
		}
	}

	static std::string static_str;
	static_str = osstream.str();
	
	return static_str.c_str();
}

const char*ina::grid::writeCube(const ina::query_model::Query& query, JSONWriter& writer)
{
	using namespace wasabi;

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
			std::cout << "WASABI: dev. warnings hardcoded answer " << std::endl;
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
			std::cerr << "WASABI: Analytics expnad cube NYI, please use datasource objectname='Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma'" << std::endl;
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
		throw std::runtime_error("InA_Interpreter => Unsupported InA request, only expand cube for metadat query is supported");

	return nullptr;
}


//TODO: move the following code  to Grid
void ina::grid::writeGrid(const ina::query_model::Query& query, JSONWriter& writer)
{
	const ina::query_model::Dimension* measDim = nullptr; // TODO: Workaround waiting to have a grid
	std::vector<const ina::query_model::Dimension*> rowAxe; // TODO: Workaround waiting to have a grid
	std::vector<const ina::query_model::Dimension*> colAxe; // TODO: Workaround waiting to have a grid
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
		dbproxy::DBProxy::getDBProxy(cnxString)->executeSQL(queryGen.getSQL(*data), &lambda);

		cube.setStorage(data);
		for (const auto& dimension : query.getDefinition().getDimensions()) 
		{
			if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
				rowAxe.push_back(&dimension);
			else if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
				colAxe.push_back(&dimension);

			if(!ina::query_model::Dimension::isDimensionOfMeasures(dimension) )
			{
				if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
					cube.addDim(calculator::eAxe::Row, dimension.getName());
				else if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
					cube.addDim(calculator::eAxe::Column, dimension.getName());
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
			if(!rowAxe.empty())
			{
				JSON_MAP(writer);
				writer.pair("Name", "ROWS");
				writeDimensions(writer, rowAxe, cube);
				{
					writeTuples(writer, query.getDefinition(), cube, calculator::eAxe::Row);
				}
			}
			if(!colAxe.empty())
			{
				JSON_MAP(writer);
				writer.pair("Name", "COLUMNS");
				writeDimensions(writer, colAxe, cube);
				{
					writeTuples(writer, query.getDefinition(), cube, calculator::eAxe::Column);
				}
			}
		}

		writer.key("CellArraySizes");
		{
			JSON_LIST(writer);
			writer.value(cube.getBody().getRowNbrs() * (measDim && measDim->getAxe()==ina::query_model::Dimension::eAxe::Rows? measDim->getMembers().size():1 ));
			writer.value(cube.getBody().getColNbrs() * (measDim && measDim->getAxe()==ina::query_model::Dimension::eAxe::Columns? measDim->getMembers().size():1 ));
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
							for(const auto& dim : cube.getBody())
							{
								const auto& data = cube.getBody().getValue(dim.getName(), colIndex, rowIndex);
								switch (cube.getBody().getValueDatatype(dim.getName()))
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
			writer.key("CellValueTypes");
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
							for(const auto& measName : cube.getBody())
							{
								const auto& data = cube.getBody().getValue(measName.getName(), colIndex, rowIndex);
								switch (cube.getBody().getValueDatatype(measName.getName()))
								{
								case calculator::eDataType::String:
								{
									writer.value(4);
									break;
								}
								case calculator::eDataType::Number:
								{
									writer.value(10);
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
}

void ina::grid::writeDimensions(JSONWriter& writer, std::vector<const ina::query_model::Dimension*>& dims, const calculator::Cube& cube)
{
	writer.key("Dimensions");
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

void ina::grid::writeTuples(JSONWriter& writer, const ina::query_model::Definition & definition , const calculator::Cube& cube, calculator::eAxe eAxe)
{
	writer.key("Tuples");
	JSON_LIST(writer);
	{
		auto axis 		= cube.getAxe(eAxe);
		for(auto dimension  : definition.getDimensions())
		{
			if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows && eAxe != calculator::eAxe::Row)
				continue;

			if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns && eAxe != calculator::eAxe::Column)
				continue;
				
			JSON_MAP(writer);
			{
				writer.key("TupleElementIds");
				{
					writeTupleValues(writer, axis, dimension);
				}
				writer.key("MemberIndexes");
				{
					JSON_MAP(writer);
					writer.pair("Encoding","None");
					writer.key("Values");
					{
						JSON_LIST(writer);
						if(ina::query_model::Dimension::isDimensionOfMeasures(dimension) )
						{
							size_t index = 0;
							for(const auto& member : dimension.getMembers())
								writer.value(index++);
						}
						else
						{
							for (size_t rowIndex = 0; rowIndex < axis.getCardinality(); rowIndex++) 
								writer.value(rowIndex);
	
						}
					}
				}
			}
		}
	}
}

void ina::grid::writeTupleValues(JSONWriter &writer, const calculator::Axe& axis, const ina::query_model::Dimension& dimension) 
{
	JSON_MAP(writer);
	writer.pair("Encoding","None");
	writer.key("Values");
	JSON_LIST(writer);
	if(ina::query_model::Dimension::isDimensionOfMeasures(dimension) )
	{
		for(const auto& member : dimension.getMembers())
			writer.value(member.getName());
	}
	else
	{
		const std::string& dimName = dimension.getName();
		for (size_t rowIndex = 0; rowIndex < axis.getCardinality(); rowIndex++) 
		{
			auto valueDataType = axis.getValueDatatype(dimName);
			auto value = axis.getValue(dimName, rowIndex);
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
}
