#include "query_generator.h"

#include <iostream>

#include "test_tools/TestAssert.h"

int main()
{
	TEST_INIT();

	query_model::InA_query_model queryModel;

	queryModel.setTable("MyTable");
	queryModel.addDim("col_A", "String");
	queryModel.addDim("col_B", "String");
	queryModel.addMeas("col_C", "Integer", "SUM");

	const query_generator::query_generator& queryGen = query_generator::query_generator(queryModel);
	std::string sql = queryGen.getSQL();

	std::cout << "Generated SQL: " << sql << std::endl;

	CPPUNIT_ASSERT_EQUAL("SELECT col_A, col_B, SUM(col_C) FROM MyTable;", sql);
	
	return TEST_HAVEERROR();
}
