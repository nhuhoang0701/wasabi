#pragma once

class Cube
{
public:
	Cube();
	
	void prepare(const query_generator& query);

	void insertRow(const Row& row);
};
