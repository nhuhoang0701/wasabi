#include "InA_DataSource.h"

#include <json/jsonWriter.h>

namespace InA_DataSource
{
	void write(const DataSource& ds, JSONWriter& writer)
	{
		writer.key("DataSource");
		{
			JSON_MAP(writer);
			writer.pair("Alias", ds.getObjectName());
			writer.pair("InstanceId", ds.getInstanceId());
			writer.pair("ObjectName", ds.getObjectName());
			if (!ds.getPackageName().empty())
				writer.pair("PackageName", ds.getPackageName());
			if (!ds.getSchemaName().empty())
				writer.pair("SchemaName", ds.getSchemaName());
			writer.pair("Type", DataSource::toString(ds.getType()));
		}
	}
}
