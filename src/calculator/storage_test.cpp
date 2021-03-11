#include "storage.h"

#include "dbproxy/dbproxy.h"

#include "test_tools/TestAssert.h"

int main()
{
	TEST_INIT();
	
	using namespace calculator;

	DataStorage storage;
	CPPUNIT_ASSERT_EQUAL(0,storage.size());

	storage.addColumn(eDataType::String, eColumnType::Indexed);
	CPPUNIT_ASSERT_EQUAL(1,storage.size());

	storage.addColumn(eDataType::String, eColumnType::NoneIndexed);
	CPPUNIT_ASSERT_EQUAL(2,storage.size());

	const auto& col0 = storage[0];
	{
		dbproxy::Row row;
		row.emplace_back("col0val0");
		row.emplace_back("col1val0");
		storage.insertRow(row);
		CPPUNIT_ASSERT_EQUAL(1,std::get<ColumnIndexed>(std::get<2>(col0)).getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(1,std::get<ColumnIndexed>(std::get<2>(col0)).size());
	}
	{
		dbproxy::Row row;
		row.emplace_back("col0val1");
		row.emplace_back("col1val1");
		storage.insertRow(row);
		CPPUNIT_ASSERT_EQUAL(2,std::get<ColumnIndexed>(std::get<2>(col0)).getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(2,std::get<ColumnIndexed>(std::get<2>(col0)).size());
	}
	{
		dbproxy::Row row;
		row.emplace_back("col0val0");
		row.emplace_back("col1val0");
		storage.insertRow(row);
		CPPUNIT_ASSERT_EQUAL(2,std::get<ColumnIndexed>(std::get<2>(col0)).getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(3,std::get<ColumnIndexed>(std::get<2>(col0)).size());
	}
	{
		dbproxy::Row row;
		row.emplace_back("col0val1");
		row.emplace_back("col1val1");
		storage.insertRow(row);
		CPPUNIT_ASSERT_EQUAL(2,std::get<ColumnIndexed>(std::get<2>(col0)).getNbDistinctVals());
		CPPUNIT_ASSERT_EQUAL(4,std::get<ColumnIndexed>(std::get<2>(col0)).size());
	}
	
	return TEST_HAVEERROR();
}
