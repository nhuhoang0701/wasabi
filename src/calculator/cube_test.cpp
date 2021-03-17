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
	storage->addColumn("Dim", eDataType::String, eColumnType::Indexed);
	storage->addColumn("DimA", eDataType::String, eColumnType::Indexed);
	storage->addColumn("DimB", eDataType::String, eColumnType::Indexed);
	storage->addColumn("Meas0", eDataType::Number, eColumnType::NoneIndexed);
	storage->addColumn("Meas1", eDataType::Number, eColumnType::NoneIndexed);
	{
		storage->insertRow({"A", "val0","val0","1","2"});
		storage->insertRow({"B", "val1","val0","2","2"});
		storage->insertRow({"C", "val1","val1","3","2"});
		storage->insertRow({"D", "val1","val2","3","2"});
	}

	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(Cube::eAxe::Row, Object("Dim"));

		CPPUNIT_ASSERT_EQUAL(4, cube.getAxe(Cube::eAxe::Row).getCardinality());
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(Cube::eAxe::Column).getCardinality());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getCellsNbs());
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addMeas("Meas0");
		cube.addMeas("Meas1");

		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(Cube::eAxe::Row).getCardinality());
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(Cube::eAxe::Column).getCardinality());
		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().getCellsNbs());
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(Cube::eAxe::Row, Object("DimA"));
		cube.addDim(Cube::eAxe::Column, Object("DimB"));
		cube.addMeas("Meas0");
		cube.addMeas("Meas1");

		CPPUNIT_ASSERT_EQUAL(2, cube.getAxe(Cube::eAxe::Row).getCardinality());
		CPPUNIT_ASSERT_EQUAL(3, cube.getAxe(Cube::eAxe::Column).getCardinality());
		CPPUNIT_ASSERT_EQUAL(12, cube.getBody().getCellsNbs());
	}

	return TEST_HAVEERROR();
}
