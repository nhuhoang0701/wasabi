#include "cube.h"
#include "storage.h"

#include <dbproxy/dbproxy.h>

#include <test_tools/TestAssert.h>
#include <json/jsonReader.h>

#include <memory>

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
		cube.addMeasure(Object("Meas0"));
		cube.addMeasure(Object("Meas1"));
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(calculator::eAxe::Row).getCardinality());
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());

		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getCellCount());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getColumnCount());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getRowCount());
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
		cube.addDim(calculator::eAxe::Row, Object("Dim"));
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(4, cube.getAxe(calculator::eAxe::Row).getCardinality());
			CPPUNIT_ASSERT_EQUAL("A", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("Dim", 0)) );
			CPPUNIT_ASSERT_EQUAL("B", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("Dim", 1)) );
			CPPUNIT_ASSERT_EQUAL("C", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("Dim", 2)) );
			CPPUNIT_ASSERT_EQUAL("D", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("Dim", 3)) );
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());

		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getCellCount());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getColumnCount());
		CPPUNIT_ASSERT_EQUAL(0, cube.getBody().getRowCount());
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addMeasure(Object("Meas0"));
		cube.addMeasure(Object("Meas1"));
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(calculator::eAxe::Row).getCardinality());
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());

		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().getCellCount());
		CPPUNIT_ASSERT_EQUAL(1, cube.getBody().getColumnCount());
		CPPUNIT_ASSERT_EQUAL(1, cube.getBody().getRowCount());
		CPPUNIT_ASSERT_EQUAL(calculator::eDataType::Number, cube.getBody().getMeasure("Meas0").getDataType());
		CPPUNIT_ASSERT_EQUAL(9, std::get<double>(cube.getBody().getValue("Meas0", 0, 0)));
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(calculator::eAxe::Row, Object("DimA"));
		cube.addDim(calculator::eAxe::Row, Object("DimB"));
		cube.addMeasure(Object("Meas0"));
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(4, cube.getAxe(calculator::eAxe::Row).getCardinality());
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimA", 0)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimA", 1)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimA", 2)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimA", 3)) );
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimB", 0)) );
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimB", 1)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimB", 2)) );
			CPPUNIT_ASSERT_EQUAL("val2", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimB", 3)) );
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());

		CPPUNIT_ASSERT_EQUAL(1, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(4, cube.getBody().getCellCount());
		CPPUNIT_ASSERT_EQUAL(1, cube.getBody().getColumnCount());
		CPPUNIT_ASSERT_EQUAL(4, cube.getBody().getRowCount());
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(calculator::eAxe::Row, Object("DimA"));
		cube.addDim(calculator::eAxe::Column, Object("DimB"));
		cube.addMeasure(Object("Meas0"));
		cube.addMeasure(Object("Meas1"));
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(2, cube.getAxe(calculator::eAxe::Row).getCardinality());
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimA", 0)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimA", 1)) );
		CPPUNIT_ASSERT_EQUAL(3, cube.getAxe(calculator::eAxe::Column).getCardinality());
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(calculator::eAxe::Column).getValue("DimB", 0)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(calculator::eAxe::Column).getValue("DimB", 1)) );
			CPPUNIT_ASSERT_EQUAL("val2", std::get<std::string>(cube.getAxe(calculator::eAxe::Column).getValue("DimB", 2)) );

		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(12, cube.getBody().getCellCount());
		CPPUNIT_ASSERT_EQUAL(3, cube.getBody().getColumnCount());
		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().getRowCount());
			CPPUNIT_ASSERT_EQUAL("##NULL##", std::get<std::string>(cube.getBody().getValue("Meas1", 1, 0)));
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(calculator::eAxe::Row, Object("Dim"));
		cube.addDim(calculator::eAxe::Row, Object("DimA"));
		cube.addDim(calculator::eAxe::Row, Object("DimB"));
		cube.addMeasure(Object("Meas0"));
		cube.addMeasure(Object("Meas1"));
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(4, cube.getAxe(calculator::eAxe::Row).getCardinality());
			CPPUNIT_ASSERT_EQUAL("A", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("Dim", 0)) );
			CPPUNIT_ASSERT_EQUAL("B", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("Dim", 1)) );
			CPPUNIT_ASSERT_EQUAL("C", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("Dim", 2)) );
			CPPUNIT_ASSERT_EQUAL("D", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("Dim", 3)) );
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimA", 0)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimA", 1)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimA", 2)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimA", 3)) );
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimB", 0)) );
			CPPUNIT_ASSERT_EQUAL("val0", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimB", 1)) );
			CPPUNIT_ASSERT_EQUAL("val1", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimB", 2)) );
			CPPUNIT_ASSERT_EQUAL("val2", std::get<std::string>(cube.getAxe(calculator::eAxe::Row).getValue("DimB", 3)) );
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());

		CPPUNIT_ASSERT_EQUAL(2, cube.getBody().size());
		CPPUNIT_ASSERT_EQUAL(8, cube.getBody().getCellCount());
		CPPUNIT_ASSERT_EQUAL(4, cube.getBody().getRowCount());
		CPPUNIT_ASSERT_EQUAL(1, cube.getBody().getColumnCount());
			CPPUNIT_ASSERT_EQUAL(calculator::eDataType::Number, cube.getBody().getMeasure("Meas0").getDataType());
			CPPUNIT_ASSERT_EQUAL(calculator::eDataType::String, cube.getBody().getMeasure("Meas1").getDataType());
			CPPUNIT_ASSERT_EQUAL(1,        std::get<double>(cube.getBody().getValue("Meas0", 0, 0)));
			CPPUNIT_ASSERT_EQUAL(3,        std::get<double>(cube.getBody().getValue("Meas0", 0, 3)));
			CPPUNIT_ASSERT_EQUAL("2", std::get<std::string>(cube.getBody().getValue("Meas1", 0, 0)));
			CPPUNIT_ASSERT_EQUAL("2", std::get<std::string>(cube.getBody().getValue("Meas1", 0, 3)));
	}

	{
		JSONReader reader;
		
		ina::query_model::Formula formula1;
		read(formula1, reader.parse(R"({"Name":"fct1", "Function":{"Name": "+","Parameters": [{"Constant": {"ValueType": "String","Value": "9"}},{"Member": {"Name": "Meas0"}}]}})"));

		ina::query_model::Formula formula2;
		read(formula2, reader.parse(R"({"Name":"fct2", "Function":{"Name": "+","Parameters": [{"Constant": {"ValueType": "String","Value": "7"}},{"Member": {"Name": "fct1"}}]}}})"));

		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(calculator::eAxe::Row, Object("Dim"));
		cube.addFormula(Object("fct1"), formula1);
		cube.addFormula(Object("fct2"), formula2);
		cube.materialyze();

		CPPUNIT_ASSERT_EQUAL(4, cube.getAxe(calculator::eAxe::Row).getCardinality());
			CPPUNIT_ASSERT_EQUAL(1, std::get<double>(cube.getBody().getValue("Meas0", 0, 0)) );
			CPPUNIT_ASSERT_EQUAL(1+9, std::get<double>(cube.getBody().getValue("fct1", 0, 0)) );
			CPPUNIT_ASSERT_EQUAL(1+9+7, std::get<double>(cube.getBody().getValue("fct2", 0, 0)) );
			CPPUNIT_ASSERT_EQUAL(2, std::get<double>(cube.getBody().getValue("Meas0", 0, 1)) );
			CPPUNIT_ASSERT_EQUAL(2+9, std::get<double>(cube.getBody().getValue("fct1", 0, 1)) );
			CPPUNIT_ASSERT_EQUAL(2+9+7, std::get<double>(cube.getBody().getValue("fct2", 0, 1)) );
		CPPUNIT_ASSERT_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());
	}

	return TEST_HAVEERROR();
}
