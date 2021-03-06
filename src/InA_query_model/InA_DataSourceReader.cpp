#include "InA_DataSource.h"

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
		{
			DataSource::Type type = DataSource::fromString(dataSource.getString("Type"));
			ds.setType(type);
		}
		else if(ds.getObjectName() == "$$DataSource$$")
			ds.setType(DataSource::Type::TypeCatalog);


		if (JSONGenericObject customProperties = dataSource.getObject("CustomProperties"))
		{
			{
				const std::string querySpecJSON = customProperties.getString("sourceQuery");
				JSONReader reader;
				JSONGenericObject root = reader.parse(querySpecJSON);
			}
		}
	}
} // InA_DataSource
