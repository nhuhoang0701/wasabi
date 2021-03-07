#include "Query.h"
#include "Definition.h"
#include "DataSource.h"

#include <test_tools/TestAssert.h>

#include <iostream>
#include <sstream>


int main()
{
	TEST_INIT();
	
	ina::query_model::Query query;

	ina::query_model::DataSource ds;
	ds.setObjectName("MyTable");
	
	ina::query_model::Definition definition;

	definition.addDimension(ina::query_model::Dimension("Dim_A", "Rows"));
	definition.addDimension(ina::query_model::Dimension("Dim_B", "Rows"));
	
	ina::query_model::Dimension dimensionMeasure("Meas_1", "Rows");
	dimensionMeasure.addMember(ina::query_model::Member("Meas_1", "Sum"));
	definition.addDimension(dimensionMeasure);

	CPPUNIT_ASSERT_EQUAL("MyTable", ds.getObjectName());
	CPPUNIT_ASSERT_EQUAL(3, definition.getDimensions().size());
	CPPUNIT_ASSERT_EQUAL("Dim_A", definition.getDimensions()[0].getName());
	CPPUNIT_ASSERT_EQUAL("Sum", definition.getDimensions()[2].getMembers()[0].getAggregation());

	return TEST_HAVEERROR();
}
