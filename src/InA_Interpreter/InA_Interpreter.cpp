#include "InA_Interpreter.h"

#include <sstream>
#include <iostream>

#include "server_info_response.h"
#include "InA_query_model.h"
#include "query_generator.h"

#define WASM_EXPORT extern "C"

std::string static_str;

WASM_EXPORT
const char* json_getServerInfo()
{
	// std::cout << "InA_Interpreter => getServerInfo call received" << std::endl;
	return server_info_json_response.c_str();
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

		processRequest(root, writer);
	}
	static_str = osstream.str();
	
	return static_str.c_str();
}

void processRequest(const JSONGenericObject& topElement, JSONWriter& writer)
{
		// std::cout << "InA_Interpreter => processRequest" << std::endl;

		query_model::InA_query_model queryModel;
		queryModel.setTable("Table1");

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
		}
		else if(topElement.haveValue("Analytics"))
		{
			JSON_LIST(writer);
			// std::cout << "InA_Interpreter => Process 'Analytics' InA request" << std::endl;
			writer.value("Process 'Analytics' InA request");

			auto& analytics = topElement.getObject("Analytics");
			if(analytics.haveValue("Definition"))
			{
				auto& definition = analytics.getObject("Definition");
				if(definition.haveValue("Dimensions"))
				{
					auto& dims = definition.getArray("Dimensions");
					for(int i = 0;i < dims.size();i++)
					{
						const std::string& dim = dims[i].getString("Name");
						// std::cout << "InA_Interpreter => requested dimension " << dim << std::endl;
						queryModel.addDim(dim, "String");
					}
				}
			}
			const query_generator::query_generator& queryGen = query_generator::query_generator(queryModel);
			const std::string sql = queryGen.getSQL();
			std::cout << "InA_Interpreter => Generated SQL: " << sql << std::endl;
			writer.value("SQL = " + sql);
		}
		else
		{
			std::cerr << "InA_Interpreter => Unsupported InA request" << std::endl;
			writer.value("Error: unsupported InA request");
		}
}
