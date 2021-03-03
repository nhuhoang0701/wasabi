#include "InA_query_model.h"

#include <json/jsonReader.h>
#include <json/jsonWriter.h>
#include <test_tools/TestAssert.h>

#include <iostream>
#include <sstream>

const std::string serialize(const query_model::InA_query_model queryModel);
const query_model::InA_query_model deserialize(const std::string& serialized);

int main()
{
	TEST_INIT();

	query_model::InA_query_model queryModel;

	queryModel.setTable("MyTable");
	query_model::InA_dimension dimensionA("Dim_A", query_model::InA_dimension::Type::ObjectsDimension, "String");
	queryModel.addDimension(dimensionA);
	query_model::InA_dimension dimensionB("Dim_B", query_model::InA_dimension::Type::ObjectsDimension, "String");
	queryModel.addDimension(dimensionB);
	query_model::InA_dimension dimensionMeasure("Meas_1", query_model::InA_dimension::Type::MeasuresDimension, "String");
	query_model::InA_member measure1("Meas_1", "Numeric", "Sum");
	dimensionMeasure.addMember(measure1);
	queryModel.addDimension(dimensionMeasure);

	CPPUNIT_ASSERT_EQUAL("MyTable", queryModel.getTable());
	CPPUNIT_ASSERT_EQUAL(3, queryModel.getObjects().size());
	CPPUNIT_ASSERT_EQUAL("Dim_A", queryModel.getObjects()[0].getName());
	CPPUNIT_ASSERT_EQUAL("String", queryModel.getObjects()[1].getDatatype());
	CPPUNIT_ASSERT_EQUAL("Sum", queryModel.getObjects()[2].getMembers()[0].getAggregation());

	const std::string serialized = serialize(queryModel);
	//std::cout << "Serialized QueryModel : " << serialized << std::endl;
	query_model::InA_query_model deserialized = deserialize(serialized);

	CPPUNIT_ASSERT_EQUAL("MyTable", deserialized.getTable());
	CPPUNIT_ASSERT_EQUAL(3, deserialized.getObjects().size());
	CPPUNIT_ASSERT_EQUAL("Dim_A", queryModel.getObjects()[0].getName());
	CPPUNIT_ASSERT_EQUAL("String", queryModel.getObjects()[1].getDatatype());
	CPPUNIT_ASSERT_EQUAL("Sum", queryModel.getObjects()[2].getMembers()[0].getAggregation());

	return TEST_HAVEERROR();
}

const std::string serialize(const query_model::InA_query_model queryModel)
{
	std::ostringstream osstream;
	JSONWriter writer(osstream);
	queryModel.write(writer);
	return osstream.str();
}

const query_model::InA_query_model deserialize(const std::string& serialized)
{
	JSONReader reader;
	const JSONGenericObject& json = reader.parse(serialized);
	return query_model::InA_query_model(json);
}


