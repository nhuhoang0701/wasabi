#include "Query.h"
#include "Definition.h"
#include "DataSource.h"
#include <json/jsonReader.h>
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

	{
		std::string request = R"({"Name":"dimName","Axis":"Rows","Attributes":[{"Name":"ATTR0","Obtainability":"UserInterface"}, {"Name":"ATTR1","Obtainability":"UserInterface"}]})";
		JSONReader reader;
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Dimension dimension;
		read(dimension, root);

		CPPUNIT_ASSERT_EQUAL(2, dimension.getAttributes().size());
		CPPUNIT_ASSERT_EQUAL_STR("ATTR0", dimension.getAttributes().at(0).getName().c_str());
		CPPUNIT_ASSERT_EQUAL_STR("ATTR1", dimension.getAttributes().at(1).getName().c_str());
    }
	{
		std::string request = R"({"Dimensions":[{"Name":"text","Axis":"Rows"},{"Name":"varchar","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Cols","Members":[{"Description":"Measure 1","Name":"real", "Aggregation":"SUM"}]}]})";
		JSONReader reader;
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		CPPUNIT_ASSERT_EQUAL(3, definition.getDimensions().size());

	}

	{
		std::string request = R"({"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"=","Low":"OBJ_147"},{"Comparison":"<>","Low":"OBJ_191"}],"FieldName":"[Measures].[Measures]"}}]}}}})";
		JSONReader reader;
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		CPPUNIT_ASSERT_EQUAL(2, definition.getQueryFilters().size());
		
		CPPUNIT_ASSERT_EQUAL("[Measures].[Measures]", definition.getQueryFilters().at(0).getFieldName());
		CPPUNIT_ASSERT_EQUAL(ina::query_model::InA_queryFilter::ComparisonOperator::EqualTo, definition.getQueryFilters().at(0).getComparisonOperator());
		CPPUNIT_ASSERT_EQUAL("OBJ_147", definition.getQueryFilters().at(0).getLowValue());
		
		CPPUNIT_ASSERT_EQUAL("[Measures].[Measures]", definition.getQueryFilters().at(1).getFieldName());
		CPPUNIT_ASSERT_EQUAL(ina::query_model::InA_queryFilter::ComparisonOperator::NotEqualTo, definition.getQueryFilters().at(1).getComparisonOperator());
		CPPUNIT_ASSERT_EQUAL("OBJ_191", definition.getQueryFilters().at(1).getLowValue());
	}
	

	return TEST_HAVEERROR();
}
