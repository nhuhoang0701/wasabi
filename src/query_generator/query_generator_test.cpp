#include "query_generator.h"

#include <iostream>

#include "test_tools/TestAssert.h"

int main()
{
	TEST_INIT();

	query_model::InA_query_model queryModel;
	queryModel.setTable("MyTable");

	const query_generator::query_generator& queryGen = query_generator::query_generator(queryModel);
	std::string sql = queryGen.getSQL();
	std::cout << "Generated SQL: " << sql << "\n";

	CPPUNIT_ASSERT_EQUAL("SELECT colA, colB, colC FROM MyTable;", sql);
	
	return TEST_HAVEERROR();
}
