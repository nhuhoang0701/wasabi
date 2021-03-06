#pragma once

#include <string>

namespace cube {class Cube;}
namespace ina::query_model {class Query;}

namespace query_generator
{

	class query_generator
	{
	public:
		query_generator(const ina::query_model::Query& query) : m_query(query) {};

		void prepareCube(cube::Cube& cube) const;

		std::string getSQL() const;

	private:
		const ina::query_model::Query& m_query;
	};
} // query_generator
