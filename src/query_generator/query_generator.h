#pragma once

#include <string>

#include "InA_query_model.h"

namespace cube {class Cube;}

namespace query_generator
{

	class query_generator
	{
	public:
		query_generator(const query_model::InA_query_model& qryModel) : m_qryModel(qryModel) {};

		void prepareCube(cube::Cube& cube);

		std::string getSQL() const;

	private:
		const query_model::InA_query_model& m_qryModel;
	};
} // query_generator
