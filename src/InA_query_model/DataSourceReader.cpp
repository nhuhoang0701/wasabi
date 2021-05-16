#include "DataSource.h"

#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{
	void read(DataSource &ds, const JSONGenericObject& dataSource)
	{
		ds.setObjectName(dataSource.getString("ObjectName"));

		if (dataSource.haveValue("InstanceId"))
			ds.setInstanceId(dataSource.getString("InstanceId"));

		if (dataSource.haveValue("PackageName"))
			ds.setPackageName(dataSource.getString("PackageName"));

		if (dataSource.haveValue("Type"))
			ds.setType(DataSource::fromString(dataSource.getString("Type")));

		if (JSONGenericObject customProperties = dataSource.getObject("CustomProperties"))
		{
			//const std::string querySpecJSON = customProperties.getString("sourceQuery");
			//JSONReader reader;
			//JSONGenericObject root = reader.parse(querySpecJSON);
		}
	}
} // ina::query_model
