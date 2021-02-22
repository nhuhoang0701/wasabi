#pragma once

#include <string>

class InA_query_model;
class Cube;

class query_generator
{
public:
	query_generator(const InA_query_model& qryModel);

	void prepareCube(Cube& cube);

	std::string   getSQL();
};
