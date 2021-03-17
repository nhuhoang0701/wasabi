#include "storage.h"

#include <dbproxy/dbproxy.h>

#include <test_tools/TestAssert.h>

int main()
{
	TEST_INIT();
	
	using namespace calculator;

	DataStorage storage;
	CPPUNIT_ASSERT_EQUAL(0,storage.getColNbrs());

	storage.addColumn("col0", eDataType::String, eColumnType::Indexed);
	CPPUNIT_ASSERT_EQUAL(1,storage.getColNbrs());

	storage.addColumn("col1", eDataType::Number, eColumnType::NoneIndexed);
	CPPUNIT_ASSERT_EQUAL(2,storage.getColNbrs());

	CPPUNIT_ASSERT_EQUAL(0,storage.getColIndex("col0"));
	CPPUNIT_ASSERT_EQUAL(1,storage.getColIndex("col1"));

	const auto& col0 = storage[0];
	CPPUNIT_ASSERT_EQUAL(eDataType::String,std::get<0>(col0));
	CPPUNIT_ASSERT_EQUAL(eColumnType::Indexed,std::get<1>(col0));

	const auto& col1 = storage[1];
	CPPUNIT_ASSERT_EQUAL(eDataType::Number,std::get<0>(col1));
	CPPUNIT_ASSERT_EQUAL(eColumnType::NoneIndexed,std::get<1>(col1));

	const auto& col0Data = std::get<2>(col0);
	const auto& col1Data = std::get<2>(col1);
	{
		storage.insertRow({"col0val0", "1.0"});
		CPPUNIT_ASSERT_EQUAL(1,col0Data->getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(1,col1Data->getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(1,col0Data->getNbVals());
		CPPUNIT_ASSERT_EQUAL(1,col1Data->getNbVals());
		CPPUNIT_ASSERT_EQUAL("col0val0",std::get<std::string>((*col0Data)[0]));
		CPPUNIT_ASSERT_EQUAL(1.0,std::get<double>((*col1Data)[0]));
	}
	{
		storage.insertRow({"col0val1", "1.0"});
		CPPUNIT_ASSERT_EQUAL(2,col0Data->getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(2,col1Data->getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(2,col0Data->getNbVals());
		CPPUNIT_ASSERT_EQUAL(2,col1Data->getNbVals());
	}
	{
		storage.insertRow({"col0val0", "2"});
		CPPUNIT_ASSERT_EQUAL(2,col0Data->getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(3,col1Data->getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(3,col0Data->getNbVals());
		CPPUNIT_ASSERT_EQUAL(3,col1Data->getNbVals());
	}
	{
		storage.insertRow({"col0val1", "3.1"});
		CPPUNIT_ASSERT_EQUAL(2,col0Data->getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(4,col1Data->getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(4,col0Data->getNbVals());
		CPPUNIT_ASSERT_EQUAL(4,col1Data->getNbVals());
		CPPUNIT_ASSERT_EQUAL("col0val1",std::get<std::string>((*col0Data)[3]));
		CPPUNIT_ASSERT_EQUAL(3.1,std::get<double>((*col1Data)[3]));
	}
	
	return TEST_HAVEERROR();
}
