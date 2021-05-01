#include "storage.h"

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

	const auto& col0Data = storage.getColumn(0);
	CPPUNIT_ASSERT_EQUAL(eDataType::String,col0Data->getDataType());
	CPPUNIT_ASSERT(dynamic_cast<ColumnIndexed*>(col0Data.get()) != nullptr);

	const auto& col1Data = storage.getColumn(1);
	CPPUNIT_ASSERT_EQUAL(eDataType::Number,col1Data->getDataType());
	CPPUNIT_ASSERT(dynamic_cast<ColumnNoneIndexed*>(col1Data.get()) != nullptr);

	{
		storage.insertRow({"col0val0", "1.0"});
		CPPUNIT_ASSERT_EQUAL(1,col0Data->getNumberOfValues());
		CPPUNIT_ASSERT_EQUAL(1,col1Data->getNumberOfValues());
		CPPUNIT_ASSERT_EQUAL(1,col0Data->getRows());
		CPPUNIT_ASSERT_EQUAL(1,col1Data->getRows());
		CPPUNIT_ASSERT_EQUAL("col0val0",std::get<std::string>(col0Data->getValueAtRowIdx(0)));
		CPPUNIT_ASSERT_EQUAL(1.0,std::get<double>(col1Data->getValueAtRowIdx(0)));
	}
	{
		storage.insertRow({"col0val1", "1.0"});
		CPPUNIT_ASSERT_EQUAL(2,col0Data->getNumberOfValues());
		CPPUNIT_ASSERT_EQUAL(2,col1Data->getNumberOfValues());
		CPPUNIT_ASSERT_EQUAL(2,col0Data->getRows());
		CPPUNIT_ASSERT_EQUAL(2,col1Data->getRows());
	}
	{
		storage.insertRow({"col0val0", "2"});
		CPPUNIT_ASSERT_EQUAL(2,col0Data->getNumberOfValues());
		CPPUNIT_ASSERT_EQUAL(3,col1Data->getNumberOfValues());
		CPPUNIT_ASSERT_EQUAL(3,col0Data->getRows());
		CPPUNIT_ASSERT_EQUAL(3,col1Data->getRows());
	}
	{
		storage.insertRow({"col0val1", "3.1"});
		CPPUNIT_ASSERT_EQUAL(2,col0Data->getNumberOfValues());
		CPPUNIT_ASSERT_EQUAL(4,col1Data->getNumberOfValues());
		CPPUNIT_ASSERT_EQUAL(4,col0Data->getRows());
		CPPUNIT_ASSERT_EQUAL(4,col1Data->getRows());
		CPPUNIT_ASSERT_EQUAL("col0val1",std::get<std::string>(col0Data->getValueAtRowIdx(3)));
		CPPUNIT_ASSERT_EQUAL(3.1,std::get<double>(col1Data->getValueAtRowIdx(3)));
	}
	
	return TEST_HAVEERROR();
}
