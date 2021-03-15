#include "cube.h"
#include "storage.h"

#include <dbproxy/dbproxy.h>

#include <memory>
#include <test_tools/TestAssert.h>

int main()
{
	TEST_INIT();
	
	using namespace calculator;

	std::shared_ptr<DataStorage> storage(new DataStorage());
	storage->addColumn("Dim0", eDataType::String, eColumnType::Indexed);
	storage->addColumn("Dim1", eDataType::String, eColumnType::Indexed);
	storage->addColumn("Meas0", eDataType::Number, eColumnType::NoneIndexed);
	storage->addColumn("Meas1", eDataType::Number, eColumnType::NoneIndexed);
	{
		storage->insertRow({"val0","val0","1","2"});
		storage->insertRow({"val1","val0","2","2"});
		storage->insertRow({"val1","val1","3","2"});
	}

	Cube cube;
	cube.addDim(Cube::eAxe::Column, Object("Dim0"));
	cube.addDim(Cube::eAxe::Row, Object("Dim1"));
	cube.addMeas("Meas0");
	cube.addMeas("Meas1");

	cube.setStorage(storage);
	/*
	CPPUNIT_ASSERT_EQUAL(cube.getBody().size(),0);

	CPPUNIT_ASSERT_EQUAL(cube.getBody().size(),1);
	CPPUNIT_ASSERT_EQUAL(cube.getBody()[0].size(),4);
	*/
	return TEST_HAVEERROR();
}
