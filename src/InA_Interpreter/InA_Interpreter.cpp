#include "InA_Interpreter.h"
#include "InA_query_model/Parameter.h"

#include <InA_metadata/Cube.h>

#include <InA_query_model/Queries.h>
#include <InA_query_model/Query.h>
#include <InA_query_model/DataSource.h>

#include <calculator/cube.h>

#include <calculator/storage.h>
#include <query_generator/query_generator.h>

#include <InA_grid/grid.h>

#include <dbproxy/dbproxy.h>

#include <metadata_enrichment/Tables.h>

#include <json/jsonReader.h>
#include <json/jsonWriter.h>

#include <memory>
#include <fstream>      // std::ifstream
#include <sstream>
#include <iostream>
#include <ios>
#include <iomanip>
#include <stdexcept>
#include <ostream>
#include <string>
#include <vector>


extern "C"
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

void processQuery(JSONWriter& writer, const ina::query_model::Query& query);
void writeResponse(JSONWriter& writer, const ina::metadata::Cube* dsCube, const ina::grid::Grid* grid);
void getDataCube(const ina::query_model::Query& query, const ina::metadata::Cube& dsCube, calculator::Cube& cube);

extern "C"
const char* json_getResponse_json(const char* InA)
{
	JSONReader reader;
	ina::query_model::Queries queries;
	read(queries, reader.parse(InA));

	std::ostringstream osstream;
	JSONWriter writer(osstream);
	if(queries.isBatch())
	{
		JSON_MAP(writer);
		writer.key("Batch");
		{
			JSON_LIST(writer);
			for(const auto& query : queries)
				processQuery(writer, *query);

		}
	}
	else if(queries.empty() == false)
	{
		processQuery(writer, *queries[0]);
	}
	else
		throw std::runtime_error("Empty query");

	static std::string static_str;
	static_str = osstream.str();
	
	return static_str.c_str();
}

void processQuery(JSONWriter& writer, const ina::query_model::Query& query)
{
	std::unique_ptr<ina::metadata::Cube> dsCube = std::make_unique<ina::metadata::Cube>(query.getDataSource());

	calculator::Cube dataCube;
	std::unique_ptr<ina::grid::Grid> grid;

	if(query.getType() == ina::query_model::Query::qAnalytics)
	{		
		getDataCube(query, *dsCube, dataCube);
		grid = std::make_unique<ina::grid::Grid>(query, *dsCube, dataCube);
	}

	////////////////////////////////////////////
	// write the result
	writeResponse(writer, dsCube.get(), grid.get());
}

void writeResponse(JSONWriter& writer, const ina::metadata::Cube* dsCube, const ina::grid::Grid* grid)
{
	////////////////////////////////////////////
	// write the result
	JSON_MAP(writer);
	if(grid)
	{
		write(grid->getQuery().getDataSource(), writer);
		writer.key("Grids");
		JSON_LIST(writer);
		ina::grid::write(*grid, writer);
	}
	
	if(dsCube)
	{
		ina::metadata::write(*dsCube, writer);
	}
	
	writer.key("Messages");
	{
		JSON_LIST(writer);
		JSON_MAP(writer);
		writer.pair("Number", 0u);
		writer.pair("Text", "This the text of the Error 0 from root");
		writer.pair("Type", "Error");
	}
}

double getDouble(const calculator::Value& val)
{
	if(std::holds_alternative<double>(val))
		return std::get<double>(val);
	return std::stod(std::get<std::string>(val));
}

calculator::Value evalFunction(const ina::query_model::Function& fct)
{
	std::vector<calculator::Value>  paramValues(fct.getParameterCount(), std::nan("0"));
	for(size_t i = 0; i < fct.getParameterCount(); i++)
	{
		const ina::query_model::Parameter& param = fct.getParameter(i);
		switch (param.getType())
		{
			case ina::query_model::Parameter::eConstant:
			{
				if(param.getValueType()=="String")
					paramValues[i] = param.getValue();
				else
					paramValues[i] = std::stod(param.getValue());
				break;
			}
			case ina::query_model::Parameter::eFunction:
			{
				paramValues[i] = evalFunction(param.getFunction());
				break;
			}
			case ina::query_model::Parameter::eMember:
			{
				throw std::runtime_error("Member function parameter not yet implemented");
			}
			default:
				throw std::runtime_error("Unkonw funciton parameter type");
		}
	}

	switch(fct.getFunctionType())
	{
		case ina::query_model::Function::eAdd:
		{
			return getDouble(paramValues[0]) + getDouble(paramValues[1]);
			break;
		}
		case ina::query_model::Function::eSubtract:
		{
			return getDouble(paramValues[0]) - getDouble(paramValues[1]);
			break;
		}
		case ina::query_model::Function::eMultipy:
		{
			return getDouble(paramValues[0]) * getDouble(paramValues[1]);
			break;
		}
		case ina::query_model::Function::eDivide:
		{
			return getDouble(paramValues[0]) / getDouble(paramValues[1]);
			break;
		}
		case ina::query_model::Function::eSquare:
		{
			return std::pow(getDouble(paramValues[0]), getDouble(paramValues[1]));
			break;
		}
		case ina::query_model::Function::eDecFloat:
		{
			return std::stod(std::get<std::string>(paramValues[0]));
			break;
		}
		default:
			throw std::runtime_error("Unkown function parameter type");

	}
	return 42.0;
}

void getDataCube(const ina::query_model::Query& query, const ina::metadata::Cube& dsCube, calculator::Cube& cube)
{
	const query_generator::query_generator& queryGen = query_generator::query_generator(query);

	std::shared_ptr<calculator::DataStorage> data(new calculator::DataStorage());
	queryGen.prepareStorage(*data);
	std::function<void(const dbproxy::Row&)> lambda = [&data](const dbproxy::Row& row)
	{
		data->insertRow(row);
	};
	const std::string& cnxString = query.getDataSource().getPackageName();
	const std::string sql = queryGen.getSQL(*data);
	std::cout << "SQL: " << sql << std::endl;
	dbproxy::DBProxy::getDBProxy(cnxString)->executeSQL(sql, &lambda);
	std::cout << "    Nbrs of rows " << data->getRowCount() << std::endl;

	cube.setStorage(data);
	for (const auto& dimension : query.getDefinition().getDimensions()) 
	{
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
			for(const auto& member : query.getDefinition().getVisibleMembers(dimension))
			{
				if(member.getFormula() == nullptr)
					cube.addMeasure(ina::query_model::Member::getName(member));
				else if(member.getFormula() != nullptr)
					cube.addConstant(calculator::Object(member.getName()), evalFunction(member.getFormula()->getFunction()));
			}
		}
	}
	cube.materialyze();
}
