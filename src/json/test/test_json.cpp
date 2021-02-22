#include "json/jsonReader.h"

#include "test_tools/TestAssert.h"

#include <string>
#include <iostream>

#include "test_data.h"




static void Test_reader()
{
	JSONReader reader;
	JSONGenericObject root = reader.parse(Json_test_reader0);

	CPPUNIT_ASSERT(!root.getObject(""));
	CPPUNIT_ASSERT(!root.getArray(""));
	CPPUNIT_ASSERT(!root.haveValue(""));
	CPPUNIT_ASSERT(!root.isBool(""));
	CPPUNIT_ASSERT(!root.isInteger(""));
	CPPUNIT_ASSERT(!root.isDouble(""));
	CPPUNIT_ASSERT(!root.isString(""));

	LDE_CPPUNIT_EXCEPTION(root.getBool(""), std::invalid_argument);
	LDE_CPPUNIT_EXCEPTION(root.getInteger(""), std::invalid_argument);
	LDE_CPPUNIT_EXCEPTION(root.getDouble(""), std::invalid_argument);
	LDE_CPPUNIT_EXCEPTION(root.getString(""), std::invalid_argument);

	CPPUNIT_ASSERT(!root.getObject("toto"));
	CPPUNIT_ASSERT(!root.getArray("toto"));

	JSONGenericObject arrayObj = root.getObject("key").getArray("array");
	CPPUNIT_ASSERT(arrayObj);
	LDE_CPPUNIT_EXCEPTION(arrayObj.getBool(""), std::logic_error);
	LDE_CPPUNIT_EXCEPTION(arrayObj.getInteger(""), std::logic_error);
	LDE_CPPUNIT_EXCEPTION(arrayObj.getDouble(""), std::logic_error);
	LDE_CPPUNIT_EXCEPTION(arrayObj.getString(""), std::logic_error);

	CPPUNIT_ASSERT_EQUAL((size_t)10, arrayObj.size());
	LDE_CPPUNIT_EXCEPTION(arrayObj.getString(10), std::invalid_argument);

	CPPUNIT_ASSERT(!arrayObj.isNull(0));
	CPPUNIT_ASSERT(arrayObj.isString(0));
	CPPUNIT_ASSERT_EQUAL(std::string(""), arrayObj.getString(0));

	CPPUNIT_ASSERT(arrayObj.isString(1));
	CPPUNIT_ASSERT_EQUAL(std::string("string"), arrayObj.getString(1));

	CPPUNIT_ASSERT(arrayObj.isDouble(2));
	CPPUNIT_ASSERT(!arrayObj.isInteger(2));
	CPPUNIT_ASSERT_EQUAL(-1.1, arrayObj.getDouble(2));

	LDE_CPPUNIT_EXCEPTION(arrayObj.getString(3), invalid_argument);
	CPPUNIT_ASSERT(arrayObj.isInteger(3));
	CPPUNIT_ASSERT_EQUAL((int64_t)-1, arrayObj.getInteger(3));

	CPPUNIT_ASSERT(arrayObj.isBool(4));
	CPPUNIT_ASSERT_EQUAL(true, arrayObj.getBool(4));

	CPPUNIT_ASSERT(arrayObj.isBool(5));
	CPPUNIT_ASSERT_EQUAL(false, arrayObj.getBool(5));

	CPPUNIT_ASSERT(!arrayObj.isBool(6));
	CPPUNIT_ASSERT(arrayObj.isNull(6));

	CPPUNIT_ASSERT(arrayObj[7]);

	CPPUNIT_ASSERT_EQUAL((size_t)0, arrayObj[8].size());
	LDE_CPPUNIT_EXCEPTION(arrayObj[8].getString(1), std::logic_error);

	CPPUNIT_ASSERT_EQUAL((size_t)1, arrayObj[9].size());
	LDE_CPPUNIT_EXCEPTION(arrayObj[9].getString(2), std::logic_error);
	CPPUNIT_ASSERT_EQUAL((int64_t)0, arrayObj[9].getInteger(0));
	CPPUNIT_ASSERT(arrayObj[9].isInteger(0));

	JSONGenericObject map0Obj = root.getObject("key").getObject("map0");
	CPPUNIT_ASSERT(map0Obj.haveValue(""));
	CPPUNIT_ASSERT(map0Obj.haveValue("0.0"));
	CPPUNIT_ASSERT(map0Obj.haveValue("0"));
	CPPUNIT_ASSERT(map0Obj.haveValue("string"));
	CPPUNIT_ASSERT(map0Obj.haveValue("true"));
	CPPUNIT_ASSERT(map0Obj.haveValue("false"));
	CPPUNIT_ASSERT(map0Obj.haveValue("null"));

	CPPUNIT_ASSERT(map0Obj.isString(""));
	CPPUNIT_ASSERT(map0Obj.isDouble("0.0"));
	CPPUNIT_ASSERT(map0Obj.isInteger("0"));
	CPPUNIT_ASSERT(map0Obj.isString("string"));
	CPPUNIT_ASSERT(map0Obj.isBool("true"));
	CPPUNIT_ASSERT(map0Obj.isBool("false"));
	CPPUNIT_ASSERT(map0Obj.isNull("null"));
	CPPUNIT_ASSERT(map0Obj.getObject("{}"));

	LDE_CPPUNIT_EXCEPTION(map0Obj.getString("0"), invalid_argument);
	LDE_CPPUNIT_EXCEPTION(map0Obj.getInteger("0.0"), invalid_argument);
	LDE_CPPUNIT_EXCEPTION(map0Obj.getBool("0"), invalid_argument);

	CPPUNIT_ASSERT(!map0Obj.haveValue(" "));

	CPPUNIT_ASSERT(!map0Obj.isNull(""));
	CPPUNIT_ASSERT_IGNORED(!map0Obj.isInteger("0.0"));

	CPPUNIT_ASSERT_EQUAL(std::string(""), map0Obj.getString(""));
	CPPUNIT_ASSERT_EQUAL(0.0, map0Obj.getDouble("0.0"));
	CPPUNIT_ASSERT_EQUAL(0.0, map0Obj.getDouble("0"));
	CPPUNIT_ASSERT_EQUAL((int64_t)0, map0Obj.getInteger("0"));
	CPPUNIT_ASSERT_EQUAL(std::string("string"), map0Obj.getString("string"));
	CPPUNIT_ASSERT_EQUAL(true, map0Obj.getBool("true"));
	CPPUNIT_ASSERT_EQUAL(false, map0Obj.getBool("false"));

	CPPUNIT_ASSERT(!map0Obj.getArray("{}"));

	CPPUNIT_ASSERT(!map0Obj.getObject("[]"));
	CPPUNIT_ASSERT(map0Obj.getArray("[]"));
	CPPUNIT_ASSERT_EQUAL((size_t)0, map0Obj.getArray("[]").size());

	CPPUNIT_ASSERT(map0Obj.getArray("[0]"));
	CPPUNIT_ASSERT_EQUAL((size_t)1, map0Obj.getArray("[0]").size());
	CPPUNIT_ASSERT(map0Obj.getArray("[0]").isInteger(0));
	CPPUNIT_ASSERT_EQUAL((int64_t)0, map0Obj.getArray("[0]").getInteger(0));
}


int main()
{
	TEST_INIT();
	
	Test_reader();
	
	return TEST_HAVEERROR();
}
