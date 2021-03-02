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
	queryModel.addDim("Dim_A", "String");
	queryModel.addDim("Dim_B", "String");
	queryModel.addMeas("Meas_1", "Numeric", "Sum");

	CPPUNIT_ASSERT_EQUAL("MyTable", queryModel.getTable());
	CPPUNIT_ASSERT_EQUAL(3, queryModel.getObjects().size());
	CPPUNIT_ASSERT_EQUAL("Dim_A", std::get<0>(queryModel.getObjects()[0]));
	CPPUNIT_ASSERT_EQUAL("String", std::get<1>(queryModel.getObjects()[1]));
	CPPUNIT_ASSERT_EQUAL("Sum", std::get<2>(queryModel.getObjects()[2]));

	const std::string serialized = serialize(queryModel);
	std::cout << "Serialized QueryModel : " << serialized << std::endl;
	query_model::InA_query_model deserialized = deserialize(serialized);

	CPPUNIT_ASSERT_EQUAL("MyTable", deserialized.getTable());
	CPPUNIT_ASSERT_EQUAL(3, deserialized.getObjects().size());
	CPPUNIT_ASSERT_EQUAL("Dim_A", std::get<0>(deserialized.getObjects()[0]));
	CPPUNIT_ASSERT_EQUAL("String", std::get<1>(deserialized.getObjects()[1]));
	CPPUNIT_ASSERT_EQUAL("Sum", std::get<2>(deserialized.getObjects()[2]));

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


