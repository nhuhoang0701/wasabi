#include "query_generator.h"

#include <InA_query_model/InA_query.h>
#include <test_tools/TestAssert.h>

#include <iostream>

int main()
{
	TEST_INIT();

	ina::query_model::DataSource ds;
	ds.setObjectName("MyTable");

	ina::query_model::Query queryInA;
	queryInA.setDataSource(ds);
	
	ina::query_model::Definition definition;
	ina::query_model::InA_dimension dimensionA("col_A", "Rows");
	definition.addDimension(dimensionA);
	
	ina::query_model::InA_dimension dimensionB("col_B", "Rows");
	definition.addDimension(dimensionB);
	
	ina::query_model::InA_dimension dimensionMeasure("CustomDimension1", "Rows");
	ina::query_model::InA_member measure1("col_C", "SUM");
	dimensionMeasure.addMember(measure1);
	definition.addDimension(dimensionMeasure);

	queryInA.setDefinition(definition);

	const query_generator::query_generator& queryGen = query_generator::query_generator(queryInA);
	std::string sql = queryGen.getSQL();

	std::cout << "Generated SQL: " << sql << std::endl;

	CPPUNIT_ASSERT_EQUAL("SELECT col_A, col_B, SUM(col_C) FROM MyTable GROUP BY col_A, col_B;", sql);
	
	return TEST_HAVEERROR();
}
