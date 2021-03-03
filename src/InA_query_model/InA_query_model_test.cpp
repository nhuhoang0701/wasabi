#include "InA_query_model.h"

#include <test_tools/TestAssert.h>

#include <iostream>
#include <sstream>


int main()
{
	TEST_INIT();

	query_model::InA_query_model queryModel;

	queryModel.setTable("MyTable");
	query_model::InA_dimension dimensionA("Dim_A", "Rows");
	queryModel.addDimension(dimensionA);
	query_model::InA_dimension dimensionB("Dim_B", "Rows");
	queryModel.addDimension(dimensionB);
	query_model::InA_dimension dimensionMeasure("Meas_1", "Rows");
	query_model::InA_member measure1("Meas_1", "Sum");
	dimensionMeasure.addMember(measure1);
	queryModel.addDimension(dimensionMeasure);

	CPPUNIT_ASSERT_EQUAL("MyTable", queryModel.getTable());
	CPPUNIT_ASSERT_EQUAL(3, queryModel.getDimensions().size());
	CPPUNIT_ASSERT_EQUAL("Dim_A", queryModel.getDimensions()[0].getName());
	CPPUNIT_ASSERT_EQUAL("Sum", queryModel.getDimensions()[2].getMembers()[0].getAggregation());

	return TEST_HAVEERROR();
}
