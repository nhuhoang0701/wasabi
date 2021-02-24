#include "InA_Interpreter.h"

#include <sstream>
#include <iostream>

#include "server_info_response.h"

#define WASM_EXPORT extern "C"

std::string static_str;

WASM_EXPORT
const char* json_getServerInfo()
{
	std::cout << "InA_Interpreter => getServerInfo call received" << std::endl;
	return server_info_json_response.c_str();
}

WASM_EXPORT
const char* json_getResponse_json(const char* InA)
{
	std::cout << "InA_Interpreter => getResponse call received: " << InA << std::endl;
	
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

void processRequest(const JSONGenericObject& object, JSONWriter& writer)
{
		std::cout << "InA_Interpreter => processRequest" << std::endl;
		if(object.haveValue("Batch"))
		{
			std::cout << "InA_Interpreter => Process 'Batch' InA request" << std::endl;
			auto& batch = object.getArray("Batch");
			JSON_LIST(writer);
			for(int i = 0;i < batch.size();i++)
			{
				processRequest(batch[i], writer);
			}
		}
		else if(object.haveValue("Metadata"))
		{
			std::cout << "InA_Interpreter => Process 'Metadata' InA request" << std::endl;
			writer.value("Process 'Metadata' InA request");
		}
		else if(object.haveValue("Analytics"))
		{
			std::cout << "InA_Interpreter => Process 'Analytics' InA request" << std::endl;
			writer.value("Process 'Analytics' InA request");
		}
		else
		{
			std::cerr << "InA_Interpreter => Unsupported InA request" << std::endl;
			writer.value("Unsupported InA request");
		}
}
