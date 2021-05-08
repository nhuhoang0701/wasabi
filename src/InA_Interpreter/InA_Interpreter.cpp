#include "InA_Interpreter.h"
#include "InA_metadata/Cube.h"

#include <cstddef>
#include <json/jsonReader.h>
#include <json/jsonWriter.h>

#include <InA_query_model/Query.h>
#include <InA_query_model/DataSource.h>

#include <InA_grid/grid.h>

#include <ostream>

#include <dbproxy/dbproxy.h>

#include <metadata_enrichment/Tables.h>


#include <memory>
#include <fstream>      // std::ifstream
#include <sstream>
#include <iostream>
#include <ios>
#include <iomanip>
#include <stdexcept>


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

extern "C"
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
			bool haveAnal = false;
			for(const auto& query : queries)
				if(query->getType() == ina::query_model::Query::qAnalytics)
					haveAnal = true;
			if(haveAnal == true)
				write(queries[0]->getDataSource(), writer);

			{
				for(const auto& query : queries)
				{
					if(query->getType() == ina::query_model::Query::qMetadata)
					{
						const char* response = ina::metadata::writeCube(*query, writer);
						if(response != nullptr )
							return response;
					}
				}
			}
			{
				writer.key("Grids");
				JSON_LIST(writer);
				for(const auto& query : queries)
				{
					if(query->getType() == ina::query_model::Query::qAnalytics)
					{
						ina::grid::writeGrid(*query, writer);
					}
				}
			}
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

	static std::string static_str;
	static_str = osstream.str();
	
	return static_str.c_str();
}

const char* ina::metadata::writeCube(const ina::query_model::Query& query, JSONWriter& writer)
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
		else
		{
			ina::metadata::Cube cube(query.getDataSource());
			ina::metadata::write(cube, writer);
		}
	}
	else
		throw std::runtime_error("InA_Interpreter => Unsupported InA request, only expand cube for metadat query is supported");

	return nullptr;
}