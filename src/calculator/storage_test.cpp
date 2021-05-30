#include "storage.h"

#include <test_tools/TestAssert.h>

int main()
{
	TEST_INIT();
	
	using namespace calculator;

	DataStorage storage;
	WASABI_CHECK_EQUAL(0,storage.getColumnCount());

	storage.addColumn("col0", common::eDataType::String, eColumnType::Indexed);
	WASABI_CHECK_EQUAL(1,storage.getColumnCount());

	storage.addColumn("col1", common::eDataType::Numeric, eColumnType::NoneIndexed);
	WASABI_CHECK_EQUAL(2,storage.getColumnCount());

	WASABI_CHECK_EQUAL(0,storage.getColIndex("col0"));
	WASABI_CHECK_EQUAL(1,storage.getColIndex("col1"));

	const auto& col0Data = storage.getColumn(0);
	C_WASABI_CHECK_EQUAL(common::eDataType::String,col0Data->getDataType());
	WASABI_CHECK_NOTEQUAL(dynamic_cast<ColumnIndexed*>(col0Data.get()), nullptr);

	const auto& col1Data = storage.getColumn(1);
	C_WASABI_CHECK_EQUAL(common::eDataType::Numeric,col1Data->getDataType());
	WASABI_CHECK_NOTEQUAL(dynamic_cast<ColumnNoneIndexed*>(col1Data.get()), nullptr);

	{
		storage.insertRow({"col0val0", "1.0"});
		WASABI_CHECK_EQUAL(1,col0Data->getNumberOfValues());
		WASABI_CHECK_EQUAL(1,col1Data->getNumberOfValues());
		WASABI_CHECK_EQUAL(1,col0Data->getRowCount());
		WASABI_CHECK_EQUAL(1,col1Data->getRowCount());
		WASABI_CHECK_EQUAL("col0val0",col0Data->getValueAtRowIdx(0).getString());
		WASABI_CHECK_EQUAL(1.0 ,col1Data->getValueAtRowIdx(0).getDouble());
	}
	{
		storage.insertRow({"col0val1", "1.0"});
		WASABI_CHECK_EQUAL(2,col0Data->getNumberOfValues());
		WASABI_CHECK_EQUAL(2,col1Data->getNumberOfValues());
		WASABI_CHECK_EQUAL(2,col0Data->getRowCount());
		WASABI_CHECK_EQUAL(2,col1Data->getRowCount());
	}
	{
		storage.insertRow({"col0val0", "2"});
		WASABI_CHECK_EQUAL(2,col0Data->getNumberOfValues());
		WASABI_CHECK_EQUAL(3,col1Data->getNumberOfValues());
		WASABI_CHECK_EQUAL(3,col0Data->getRowCount());
		WASABI_CHECK_EQUAL(3,col1Data->getRowCount());
	}
	{
		storage.insertRow({"col0val1", "3.1"});
		WASABI_CHECK_EQUAL(2,col0Data->getNumberOfValues());
		WASABI_CHECK_EQUAL(4,col1Data->getNumberOfValues());
		WASABI_CHECK_EQUAL(4,col0Data->getRowCount());
		WASABI_CHECK_EQUAL(4,col1Data->getRowCount());
		WASABI_CHECK_EQUAL("col0val1",col0Data->getValueAtRowIdx(3).getString());
		WASABI_CHECK_EQUAL(3.1,col1Data->getValueAtRowIdx(3).getDouble());
	}
	
	return TEST_HAVEERROR();
}
