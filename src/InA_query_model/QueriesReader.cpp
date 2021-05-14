
#include "Queries.h"

#include "Query.h"

#include <json/jsonReader.h>    // For JSONGenericObject

#include <exceptions/InA_Exception.h>

#include <iostream>

namespace ina::query_model
{
	void read(Queries& queries, const JSONGenericObject& jsonQuery)
	{
		queries.m_isBatch = jsonQuery.haveValue("Batch");
		if(queries.m_isBatch)
		{
			// std::cout << "InA_Interpreter => Process 'Batch' InA request" << std::endl;
			const auto& batch = jsonQuery.getArray("Batch");
			for(int i = 0;i < batch.size();i++)
			{
				read(queries, batch[i]);
			}
		}
		else
		{
			auto query= std::make_shared<Query>();
			read(query, jsonQuery);
			queries.push_back(query);
		}			
	}
}
