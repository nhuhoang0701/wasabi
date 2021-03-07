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
	definition.addDimension(ina::query_model::InA_dimension("dim_A", "Rows"));
	
	definition.addDimension(ina::query_model::InA_dimension("dim_B", "Rows"));
	
	ina::query_model::InA_dimension dimensionMeasure("CustomDimension1", "Rows");
		dimensionMeasure.addMember(ina::query_model::InA_member("meas_1", "SUM"));
		dimensionMeasure.addMember(ina::query_model::InA_member("meas_2", "SUM"));
	definition.addDimension(dimensionMeasure);

	queryInA.setDefinition(definition);

	const query_generator::query_generator& queryGen = query_generator::query_generator(queryInA);
	std::string sql = queryGen.getSQL();

	std::cout << "Generated SQL: " << sql << std::endl;

	CPPUNIT_ASSERT_EQUAL("SELECT dim_A, dim_B, SUM(meas_1), SUM(meas_2) FROM MyTable GROUP BY dim_A, dim_B;", sql);
	
	return TEST_HAVEERROR();
}
