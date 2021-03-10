
#include "Query.h"
#include "Definition.h"
#include "DataSource.h"


#include <json/jsonReader.h>    // For JSONGenericObject

#include <exceptions/InA_Exception.h>

namespace ina::query_model
{
	void read(std::vector<std::shared_ptr<const Query>>& queries, const JSONGenericObject& jsonQuery)
	{
		if(jsonQuery.haveValue("Batch"))
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
	void read(std::shared_ptr<Query>& query, const JSONGenericObject& jsonQuery)
	{
		query->m_isExpandCube = false;
		if (JSONGenericObject metadata = jsonQuery.getObject("Metadata"))
		{
			query->m_type = Query::qMetadata;

			if (metadata.haveValue("Language"))
				query->m_language = metadata.getString("Language");
			else
				;//TODO throw TRACED_InA_EXCEPTION("Missing Language value in Metadata query");

			if (JSONGenericObject capabilities = metadata.getArray("Capabilities"))
				;//TODO
			else
				;//TODO throw TRACED_InA_EXCEPTION("Missing Capabilities object in Metadata query");

			if (JSONGenericObject datasource = metadata.getObject("DataSource"))
				read(query->m_datasource, datasource);
			else
				throw TRACED_InA_EXCEPTION("Missing DataSource object in Metadata query");

			if(const JSONGenericObject& expand = metadata.getArray("Expand"))
			{
				for(size_t i = 0; i < expand.size() ; i ++ )
				{
					if(expand.getString(i) == "Cubes")
					{
						query->m_isExpandCube = true;
						break;
					}
				}
			}
		}
		else if (JSONGenericObject analytics = jsonQuery.getObject("Analytics"))
		{
			query->m_type = Query::qAnalytics;

			if (analytics.haveValue("Language"))
				query->m_language = analytics.getString("Language");
			else
				;//TODO throw TRACED_InA_EXCEPTION("Missing Language value in Analytics query");

			if (JSONGenericObject capabilities = analytics.getArray("Capabilities"))
				;//TODO
			else
				;//TODO throw TRACED_InA_EXCEPTION("Missing Capabilities object in Analytics query");

			if (JSONGenericObject datasource = analytics.getObject("DataSource"))
				read(query->m_datasource, datasource);
			else
				;//TODO throw TRACED_InA_EXCEPTION("Missing DataSource object in Analytics query");

			if (JSONGenericObject definition = analytics.getObject("Definition"))
				read(query->m_definition, definition);
		}
		else
		{
			throw TRACED_InA_EXCEPTION("Unexpected root object in json query");
		}
	}
}
