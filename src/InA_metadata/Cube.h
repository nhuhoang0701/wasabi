#pragma once

#include "Dimension.h"

#include <vector>
#include <memory>

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

		const std::vector<std::unique_ptr<Dimension>>&  getDimensions() const;

		private:
		const ina::query_model::DataSource& m_datasource;
		std::vector<std::unique_ptr<Dimension>> m_dimensions;

		friend void write(const Cube& obj, JSONWriter& jsonNode);
	};
}
