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
	storage->addColumn("Meas1", eDataType::String, eColumnType::NoneIndexed);

	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addMeas("Meas0");
		cube.addMeas("Meas1");
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(Cube::eAxe::Row).getCardinality());
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(Cube::eAxe::Column).getCardinality());

		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getCellsNbs());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getColNbrs());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getRowNbrs());
	}

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
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(4, cube.getAxe(Cube::eAxe::Row).getCardinality());
			CPPUNIT_ASSERT_EQUAL("A", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("Dim", 0)) );
			CPPUNIT_ASSERT_EQUAL("B", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("Dim", 1)) );
			CPPUNIT_ASSERT_EQUAL("C", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("Dim", 2)) );
			CPPUNIT_ASSERT_EQUAL("D", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("Dim", 3)) );
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(Cube::eAxe::Column).getCardinality());

		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getCellsNbs());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getColNbrs());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getRowNbrs());
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addMeas("Meas0");
		cube.addMeas("Meas1");
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(Cube::eAxe::Row).getCardinality());
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(Cube::eAxe::Column).getCardinality());

		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().getCellsNbs());
		CPPUNIT_ASSERT_EQUAL(1, cube.getBody().getColNbrs());
		CPPUNIT_ASSERT_EQUAL(1, cube.getBody().getRowNbrs());
		CPPUNIT_ASSERT_EQUAL(calculator::eDataType::Number, cube.getBody().getValueDatatype("Meas0"));
		CPPUNIT_ASSERT_EQUAL(9, std::get<double>(cube.getBody().getValue("Meas0", 0, 0)));
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(Cube::eAxe::Row, Object("DimA"));
		cube.addDim(Cube::eAxe::Row, Object("DimB"));
		cube.addMeas("Meas0");
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(4, cube.getAxe(Cube::eAxe::Row).getCardinality());
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimA", 0)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimA", 1)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimA", 2)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimA", 3)) );
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimB", 0)) );
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimB", 1)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimB", 2)) );
			CPPUNIT_ASSERT_EQUAL("val2", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimB", 3)) );
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(Cube::eAxe::Column).getCardinality());

		CPPUNIT_ASSERT_EQUAL(1, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(4, cube.getBody().getCellsNbs());
		CPPUNIT_ASSERT_EQUAL(1, cube.getBody().getColNbrs());
		CPPUNIT_ASSERT_EQUAL(4, cube.getBody().getRowNbrs());
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(Cube::eAxe::Row, Object("DimA"));
		cube.addDim(Cube::eAxe::Column, Object("DimB"));
		cube.addMeas("Meas0");
		cube.addMeas("Meas1");
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(2, cube.getAxe(Cube::eAxe::Row).getCardinality());
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimA", 0)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimA", 1)) );
		CPPUNIT_ASSERT_EQUAL(3, cube.getAxe(Cube::eAxe::Column).getCardinality());
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(Cube::eAxe::Column).getValue("DimB", 0)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(Cube::eAxe::Column).getValue("DimB", 1)) );
			CPPUNIT_ASSERT_EQUAL("val2", std::get<std::string>(cube.getAxe(Cube::eAxe::Column).getValue("DimB", 2)) );

		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(12, cube.getBody().getCellsNbs());
		CPPUNIT_ASSERT_EQUAL(3, cube.getBody().getColNbrs());
		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().getRowNbrs());
			CPPUNIT_ASSERT_EQUAL("##NULL##", std::get<std::string>(cube.getBody().getValue("Meas1", 1, 0)));
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(Cube::eAxe::Row, Object("Dim"));
		cube.addDim(Cube::eAxe::Row, Object("DimA"));
		cube.addDim(Cube::eAxe::Row, Object("DimB"));
		cube.addMeas("Meas0");
		cube.addMeas("Meas1");
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(4, cube.getAxe(Cube::eAxe::Row).getCardinality());
			CPPUNIT_ASSERT_EQUAL("A", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("Dim", 0)) );
			CPPUNIT_ASSERT_EQUAL("B", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("Dim", 1)) );
			CPPUNIT_ASSERT_EQUAL("C", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("Dim", 2)) );
			CPPUNIT_ASSERT_EQUAL("D", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("Dim", 3)) );
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimA", 0)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimA", 1)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimA", 2)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimA", 3)) );
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimB", 0)) );
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimB", 1)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimB", 2)) );
			CPPUNIT_ASSERT_EQUAL("val2", std::get<std::string>(cube.getAxe(Cube::eAxe::Row).getValue("DimB", 3)) );
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(Cube::eAxe::Column).getCardinality());

		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(8, cube.getBody().getCellsNbs());
		CPPUNIT_ASSERT_EQUAL(4, cube.getBody().getRowNbrs());
		CPPUNIT_ASSERT_EQUAL(1, cube.getBody().getColNbrs());
			CPPUNIT_ASSERT_EQUAL(calculator::eDataType::Number, cube.getBody().getValueDatatype("Meas0"));
			CPPUNIT_ASSERT_EQUAL(calculator::eDataType::String, cube.getBody().getValueDatatype("Meas1"));
			CPPUNIT_ASSERT_EQUAL(1,        std::get<double>(cube.getBody().getValue("Meas0", 0, 0)));
			CPPUNIT_ASSERT_EQUAL(3,        std::get<double>(cube.getBody().getValue("Meas0", 0, 3)));
			CPPUNIT_ASSERT_EQUAL("2", std::get<std::string>(cube.getBody().getValue("Meas1", 0, 0)));
			CPPUNIT_ASSERT_EQUAL("2", std::get<std::string>(cube.getBody().getValue("Meas1", 0, 3)));
	}

	return TEST_HAVEERROR();
}
