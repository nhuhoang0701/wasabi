#include "InA_Interpreter.h"
#include "common/data.h"

#include <InA_metadata/Cube.h>

#include <InA_query_model/Queries.h>
#include <InA_query_model/Query.h>
#include <InA_query_model/QueryEx.h>
#include <InA_query_model/DataSource.h>

#include <calculator/cube.h>

#include <calculator/storage.h>
#include <cstdint>
#include <query_generator/query_generator.h>

#include <InA_grid/grid.h>

#include <dbproxy/dbproxy.h>

#include <json/jsonReader.h>
#include <json/jsonWriter.h>

#include <common/Log.h>

#include <memory>
#include <fstream>      // std::ifstream
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>


void getServerInfo(int32_t ID);
void getResponse(int32_t ID, const char* inaRequest);

extern "C"
void doIt(int32_t ID, eRequestType type, const char* inaRequest)
{
	ScopeLog sc("doIt");
	Logger::log("type", toString(type));
	Logger::log("ID", ID);
	switch (type)
	{
	case eGetServerInfo:
	{
		return getServerInfo(ID);
	}
	case eGetResponse:
	{
		return getResponse(ID, inaRequest);
	}
	case eSubmitCube:
	throw std::runtime_error("eSubmitCube not yet implemented");
	case eUndefined:
	throw std::runtime_error("Invalid request type 'eUndefine'");
	default:
	throw std::runtime_error("Unknon request type '"+std::to_string(type) +"'");
	}
}

void getServerInfo(int32_t ID)
{	
	ScopeLog sc("getServerInfo");

	static std::string static_str_getserverinfo;
	if(static_str_getserverinfo.empty() )
	{
		std::ifstream ifs("./resources/response_getSerververInfo.json");
		if(ifs.is_open() )
			static_str_getserverinfo = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		else
			throw std::runtime_error("Could not open file ./resources/response_getSerververInfo.json");
	}

	ina_callback_response(ID, eRequestType::eGetServerInfo, static_str_getserverinfo.c_str());
}

void processQuery(JSONWriter& writer, const ina::query_model::Query& query);
void writeResponse(JSONWriter& writer, const ina::metadata::Cube* dsCube, const ina::grid::Grid* grid);
std::shared_ptr<calculator::Cube> getDataCube(const ina::query_model::QueryEx& queryExec);

void getResponse(int32_t ID, const char* InA)
{
	ScopeLog sc("getResponse");
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
	
	ina_callback_response(ID, eRequestType::eGetResponse, static_str.c_str());
}

void processQuery(JSONWriter& writer, const ina::query_model::Query& query)
{
	ScopeLog sc("processQuery");
	std::unique_ptr<ina::metadata::Cube> dsCube = std::make_unique<ina::metadata::Cube>(query.getDataSource());

	std::unique_ptr<ina::grid::Grid> grid;
	ina::query_model::QueryEx queryExec(query.getDefinition(), *dsCube);

	if(query.getType() == ina::query_model::Query::qAnalytics)
	{		
		std::shared_ptr<calculator::Cube> dataCube = getDataCube(queryExec);
		grid = std::make_unique<ina::grid::Grid>(queryExec, dataCube);
	}

	////////////////////////////////////////////
	// write the result
	writeResponse(writer, dsCube.get(), grid.get());
}

void writeResponse(JSONWriter& writer, const ina::metadata::Cube* dsCube, const ina::grid::Grid* grid)
{
	ScopeLog sc("writeResponse");
	////////////////////////////////////////////
	// write the result
	JSON_MAP(writer);
	if(grid)
	{
		if(grid->getQuery().getDSCube())
			write(grid->getQuery().getDSCube()->getDataSource(), writer);
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


std::shared_ptr<calculator::DataStorage> getDataStorage(const ina::query_model::QueryEx& queryEx)
{
	ScopeLog sc("getDataStorage");
	std::shared_ptr<calculator::DataStorage> data(new calculator::DataStorage());

	const std::string catalogType = "Query";
	const auto& metadataCube = *queryEx.getDSCube();
	if(!metadataCube.getDataSource().isCatalogBrowsing())
	{
		const std::string& packageName = metadataCube.getDataSource().getPackageName();
		
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryEx);

		queryGen.prepareStorage(data);
		std::function<void(const dbproxy::Row&)> lambda = [&data](const dbproxy::Row& row)
		{
			data->insertRow(row);
		};
		const std::string sql = queryGen.getSQL();
		Logger::log("SQL: ", sql);
		dbproxy::DBProxy::getDBProxy(packageName)->executeSQL(sql, &lambda);
	}
	else
	{
        const std::string& cnxString = queryEx.getDSCube()->getDataSource().getPackageName();
		for(const auto& dim : metadataCube.getDimensions())
		{
			for(const auto& attribut : dim->getAttributes())
			{
				data->addColumn(attribut.getName(), attribut.getDataType(), calculator::eColumnType::Indexed);
			}
		}
		using namespace dbproxy;
		{
			std::string packageName = "local:sqlite:efashion";
			wasabi::metadata::Catalog catalog(packageName);
			for(const auto& tableName : catalog.getTableNames())
			{
				const auto& table = catalog.getTable(tableName);
				
				dbproxy::Row row1 = {Value(catalogType.c_str()), Value(tableName.c_str()), Value(tableName.c_str()), Value(packageName.c_str())};
				data->insertRow(row1);
			}
		}
		if(false)
		{
			std::string packageName = "local:sqlite:chinook";
			wasabi::metadata::Catalog catalog(packageName);
			for(const auto& tableName : catalog.getTableNames())
			{
				const auto& table = catalog.getTable(tableName);
				
				dbproxy::Row row1 = {Value(catalogType.c_str()), Value(tableName.c_str()), Value(tableName.c_str()), Value(packageName.c_str())};
				data->insertRow(row1);
			}
		}
	}

	Logger::log("    Nbrs of rows ", data->getRowCount());
	return data;
}


std::shared_ptr<calculator::Cube> getDataCube(const ina::query_model::QueryEx& queryEx)
{
	ScopeLog sc("getDataCube");
	
	std::shared_ptr<calculator::DataStorage> dataStorage = getDataStorage(queryEx);

	std::shared_ptr<calculator::Cube> dataCube = std::make_shared<calculator::Cube>();
	dataCube->setStorage(dataStorage);

	for (const auto& dimension : queryEx.getQueryDefinition().getDimensions()) 
	{
		if(!ina::query_model::QueryEx::isDimensionOfMeasures(dimension) )
		{
			calculator::eAxe axe;
			if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
				axe = calculator::eAxe::Row;
			else if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
				axe = calculator::eAxe::Column;
			else if (dimension.getAxe() == ina::query_model::Dimension::eAxe::None)
				continue;
			else throw std::runtime_error("Unknow axis type");
			
			for(const auto& attribut : dimension.getAttributes())
				dataCube->addDim(axe, attribut.getName());
		}
		else
		{
			// Add Measures
			for(const auto& member : queryEx.getVisibleMembers(dimension))
			{
				if(member.getFormula() == nullptr && member.getSelection() ==nullptr)
					dataCube->addMeasure(ina::query_model::Member::getName(member));
			}
			// Add formula
			for(const auto& member : queryEx.getVisibleMembers(dimension))
			{
				auto formula = calculator::Object(member.getName());
				if(member.getFormula() != nullptr && !dataCube->contain(member.getName()))
					dataCube->addFormula(formula, *member.getFormula());
			}
			// Add restriction
			for(const auto& member : queryEx.getVisibleMembers(dimension))
			{
				auto restriction = calculator::Object(member.getName());
				if(member.getSelection() != nullptr && !dataCube->contain(member.getName()))
					dataCube->addRestriction(restriction, *member.getSelection());
			}
		}
	}
	dataCube->materialyze();

	return dataCube;
}
