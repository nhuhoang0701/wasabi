#include "InA_Interpreter.h"
#include "InA_query_model/Parameter.h"
#include "calculator/common.h"

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
		/*
		JSON_MAP(writer);
		writer.pair("Number", 0u);
		writer.pair("Text", "This the text of the Error 0 from root");
		writer.pair("Type", 2u);
		*/
	}
}


void getDataCube(const ina::query_model::Query& query, const ina::metadata::Cube& dsCube, calculator::Cube& cube)
{
	const query_generator::query_generator& queryGen = query_generator::query_generator(query);

	std::shared_ptr<calculator::DataStorage> data(new calculator::DataStorage());
	queryGen.prepareStorage(*data, &dsCube);
	std::function<void(const dbproxy::Row&)> lambda = [&data](const dbproxy::Row& row)
	{
		data->insertRow(row);
	};
	const std::string& cnxString = query.getDataSource().getPackageName();
	const std::string sql = queryGen.getSQL(*data, &dsCube);
	std::cout << "SQL: " << sql << std::endl;
	dbproxy::DBProxy::getDBProxy(cnxString)->executeSQL(sql, &lambda);
	std::cout << "    Nbrs of rows " << data->getRowCount() << std::endl;

	cube.setStorage(data);
	for (const auto& dimension : query.getDefinition().getDimensions()) 
	{
		if(!ina::query_model::Dimension::isDimensionOfMeasures(dimension) )
		{
			calculator::eAxe axe;
			if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
				axe = calculator::eAxe::Row;
			else if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
				axe = calculator::eAxe::Column;
			else throw std::runtime_error("Unknow axis type");
			
			for(const auto& attribut : dimension.getAttributes())
				cube.addDim(axe, attribut.getName());
		}
		else
		{
			// Add Measures
			for(const auto member : query.getDefinition().getVisibleMembers(dimension))
			{
				if(member.getFormula() == nullptr && member.getSelection() ==nullptr)
					cube.addMeasure(ina::query_model::Member::getName(member));
			}
			// Add formula
			for(const auto member : query.getDefinition().getVisibleMembers(dimension))
			{
				auto formula = calculator::Object(member.getName());
				if(member.getFormula() != nullptr && !cube.contain(formula))
					cube.addFormula(formula, *member.getFormula());
			}
			// Add restriction
			for(const auto member : query.getDefinition().getVisibleMembers(dimension))
			{
				auto restriction = calculator::Object(member.getName());
				if(member.getSelection() != nullptr && !cube.contain(restriction))
					cube.addRestriction(restriction, *member.getSelection());
			}
		}
	}
	cube.materialyze();
}
