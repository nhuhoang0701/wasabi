#include "InA_Interpreter.h"

#include <sstream>
#include <iostream>

#include "json/jsonReader.h"
#include "json/jsonWriter.h"
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

		if(root.haveValue("Metadata"))
		{
			std::cout << "InA_Interpreter => Process 'Metadata' InA request" << std::endl;
			writer.value("Process 'Metadata' InA request");
		}
		else if(root.haveValue("Analytics"))
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
	static_str = osstream.str();
	
	return static_str.c_str();
}
