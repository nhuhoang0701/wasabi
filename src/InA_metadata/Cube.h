#pragma once

#include "InA_query_model/DataSource.h"
#include <string>

class JSONWriter;        // From <json/jsonWriter.h>

namespace ina::query_model {class DataSource;}

namespace ina::metadata
{
	class Cube;
	void write(const Cube& obj, JSONWriter& jsonNode);

	class Cube
	{
		public:
		Cube(const ina::query_model::DataSource& datasource);

		private:

		const ina::query_model::DataSource& m_datasource;

		friend void write(const Cube& obj, JSONWriter& jsonNode);
	};
}
