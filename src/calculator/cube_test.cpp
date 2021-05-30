#include "cube.h"

#include "storage.h"

#include <InA_query_model/Formula.h>
#include <InA_query_model/Selection/Selection.h>

#include <dbproxy/dbproxy.h>

#include <test_tools/TestAssert.h>
#include <json/jsonReader.h>

#include <memory>

int main()
{
	TEST_INIT();
	
	using namespace calculator;

	std::shared_ptr<DataStorage> storage(new DataStorage());
	storage->addColumn("Dim", common::eDataType::String, eColumnType::Indexed);
	storage->addColumn("DimNum", common::eDataType::Numeric, eColumnType::Indexed);
	storage->addColumn("DimA", common::eDataType::String, eColumnType::Indexed);
	storage->addColumn("DimB", common::eDataType::String, eColumnType::Indexed);
	storage->addColumn("Meas0", common::eDataType::Numeric, eColumnType::NoneIndexed);
	storage->addColumn("Meas1", common::eDataType::String, eColumnType::NoneIndexed);

	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addMeasure(Object("Meas0"));
		cube.addMeasure(Object("Meas1"));
		cube.materialyze();

		WASABI_CHECK_EQUAL(0, cube.getAxe(calculator::eAxe::Row).getCardinality());
		WASABI_CHECK_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());

		WASABI_CHECK_EQUAL(2, cube.getBody().getNumberOfVisibleObjects());
		WASABI_CHECK_EQUAL(0, cube.getBody().getCellCount());
		WASABI_CHECK_EQUAL(0, cube.getBody().getColumnCount());
		WASABI_CHECK_EQUAL(0, cube.getBody().getRowCount());
	}

	{
		storage->insertRow({"A", "0", "val0","val0","1","2"});
		storage->insertRow({"B", "1", "val1","val0","2","2"});
		storage->insertRow({"C", "2", "val1","val1","3","2"});
		storage->insertRow({"D", "3", "val1","val2","3","2"});
	}

	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(calculator::eAxe::Row, Object("Dim"));
		cube.materialyze();

		WASABI_CHECK_EQUAL(4, cube.getAxe(calculator::eAxe::Row).getCardinality());
			WASABI_CHECK_EQUAL("A", cube.getAxe(calculator::eAxe::Row).getValue("Dim", 0).getString() );
			WASABI_CHECK_EQUAL("B", cube.getAxe(calculator::eAxe::Row).getValue("Dim", 1).getString() );
			WASABI_CHECK_EQUAL("C", cube.getAxe(calculator::eAxe::Row).getValue("Dim", 2).getString() );
			WASABI_CHECK_EQUAL("D", cube.getAxe(calculator::eAxe::Row).getValue("Dim", 3).getString() );
		WASABI_CHECK_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());

		WASABI_CHECK_EQUAL(0, cube.getBody().getNumberOfVisibleObjects());
		WASABI_CHECK_EQUAL(0, cube.getBody().getCellCount());
		WASABI_CHECK_EQUAL(0, cube.getBody().getColumnCount());
		WASABI_CHECK_EQUAL(0, cube.getBody().getRowCount());
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addMeasure(Object("Meas0"));
		cube.addMeasure(Object("Meas1"));
		cube.materialyze();

		WASABI_CHECK_EQUAL(0, cube.getAxe(calculator::eAxe::Row).getCardinality());
		WASABI_CHECK_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());

		WASABI_CHECK_EQUAL(2, cube.getBody().getNumberOfVisibleObjects());
		WASABI_CHECK_EQUAL(2, cube.getBody().getCellCount());
		WASABI_CHECK_EQUAL(1, cube.getBody().getColumnCount());
		WASABI_CHECK_EQUAL(1, cube.getBody().getRowCount());
		C_WASABI_CHECK_EQUAL(common::eDataType::Numeric, cube.getBody().getObject("Meas0").getDataType());
		WASABI_CHECK_EQUAL(9.0, cube.getBody().getValue(cube.getBody().getIndex("Meas0"), 0, 0).getDouble());
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(calculator::eAxe::Row, Object("DimA"));
		cube.addDim(calculator::eAxe::Row, Object("DimB"));
		cube.addMeasure(Object("Meas0"));
		cube.materialyze();

		WASABI_CHECK_EQUAL(4, cube.getAxe(calculator::eAxe::Row).getCardinality());
			WASABI_CHECK_EQUAL("val0", cube.getAxe(calculator::eAxe::Row).getValue("DimA", 0).getString() );
			WASABI_CHECK_EQUAL("val1", cube.getAxe(calculator::eAxe::Row).getValue("DimA", 1).getString() );
			WASABI_CHECK_EQUAL("val1", cube.getAxe(calculator::eAxe::Row).getValue("DimA", 2).getString() );
			WASABI_CHECK_EQUAL("val1", cube.getAxe(calculator::eAxe::Row).getValue("DimA", 3).getString() );
			WASABI_CHECK_EQUAL("val0", cube.getAxe(calculator::eAxe::Row).getValue("DimB", 0).getString() );
			WASABI_CHECK_EQUAL("val0", cube.getAxe(calculator::eAxe::Row).getValue("DimB", 1).getString() );
			WASABI_CHECK_EQUAL("val1", cube.getAxe(calculator::eAxe::Row).getValue("DimB", 2).getString() );
			WASABI_CHECK_EQUAL("val2", cube.getAxe(calculator::eAxe::Row).getValue("DimB", 3).getString() );
		WASABI_CHECK_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());

		WASABI_CHECK_EQUAL(1, cube.getBody().getNumberOfVisibleObjects());
		WASABI_CHECK_EQUAL(4, cube.getBody().getCellCount());
		WASABI_CHECK_EQUAL(1, cube.getBody().getColumnCount());
		WASABI_CHECK_EQUAL(4, cube.getBody().getRowCount());
	}
	{
		Cube cube;	
		cube.setStorage(storage);
		cube.addDim(calculator::eAxe::Row, Object("DimA"));
		cube.addDim(calculator::eAxe::Column, Object("DimB"));
		cube.addMeasure(Object("Meas0"));
		cube.addMeasure(Object("Meas1"));
		cube.materialyze();

		WASABI_CHECK_EQUAL(2, cube.getAxe(calculator::eAxe::Row).getCardinality());
			WASABI_CHECK_EQUAL("val0", cube.getAxe(calculator::eAxe::Row).getValue("DimA", 0).getString() );
			WASABI_CHECK_EQUAL("val1", cube.getAxe(calculator::eAxe::Row).getValue("DimA", 1).getString() );
		WASABI_CHECK_EQUAL(3, cube.getAxe(calculator::eAxe::Column).getCardinality());
			WASABI_CHECK_EQUAL("val0", cube.getAxe(calculator::eAxe::Column).getValue("DimB", 0).getString() );
			WASABI_CHECK_EQUAL("val1", cube.getAxe(calculator::eAxe::Column).getValue("DimB", 1).getString() );
			WASABI_CHECK_EQUAL("val2", cube.getAxe(calculator::eAxe::Column).getValue("DimB", 2).getString() );

		WASABI_CHECK_EQUAL(2, cube.getBody().getNumberOfVisibleObjects());
		WASABI_CHECK_EQUAL(12, cube.getBody().getCellCount());
		WASABI_CHECK_EQUAL(3, cube.getBody().getColumnCount());
		WASABI_CHECK_EQUAL(2, cube.getBody().getRowCount());
			WASABI_CHECK_EQUAL("##NULL##", cube.getBody().getValue(cube.getBody().getIndex("Meas1"), 1, 0).getString());
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

		WASABI_CHECK_EQUAL(4, cube.getAxe(calculator::eAxe::Row).getCardinality());
			WASABI_CHECK_EQUAL("A", cube.getAxe(calculator::eAxe::Row).getValue("Dim", 0).getString() );
			WASABI_CHECK_EQUAL("B", cube.getAxe(calculator::eAxe::Row).getValue("Dim", 1).getString() );
			WASABI_CHECK_EQUAL("C", cube.getAxe(calculator::eAxe::Row).getValue("Dim", 2).getString() );
			WASABI_CHECK_EQUAL("D", cube.getAxe(calculator::eAxe::Row).getValue("Dim", 3).getString() );
			WASABI_CHECK_EQUAL("val0", cube.getAxe(calculator::eAxe::Row).getValue("DimA", 0).getString() );
			WASABI_CHECK_EQUAL("val1", cube.getAxe(calculator::eAxe::Row).getValue("DimA", 1).getString() );
			WASABI_CHECK_EQUAL("val1", cube.getAxe(calculator::eAxe::Row).getValue("DimA", 2).getString() );
			WASABI_CHECK_EQUAL("val1", cube.getAxe(calculator::eAxe::Row).getValue("DimA", 3).getString() );
			WASABI_CHECK_EQUAL("val0", cube.getAxe(calculator::eAxe::Row).getValue("DimB", 0).getString() );
			WASABI_CHECK_EQUAL("val0", cube.getAxe(calculator::eAxe::Row).getValue("DimB", 1).getString() );
			WASABI_CHECK_EQUAL("val1", cube.getAxe(calculator::eAxe::Row).getValue("DimB", 2).getString() );
			WASABI_CHECK_EQUAL("val2", cube.getAxe(calculator::eAxe::Row).getValue("DimB", 3).getString() );
		WASABI_CHECK_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());

		WASABI_CHECK_EQUAL(2, cube.getBody().getNumberOfVisibleObjects());
		WASABI_CHECK_EQUAL(8, cube.getBody().getCellCount());
		WASABI_CHECK_EQUAL(4, cube.getBody().getRowCount());
		WASABI_CHECK_EQUAL(1, cube.getBody().getColumnCount());
			C_WASABI_CHECK_EQUAL(common::eDataType::Numeric, cube.getBody().getObject("Meas0").getDataType());
			C_WASABI_CHECK_EQUAL(common::eDataType::String, cube.getBody().getObject("Meas1").getDataType());
			WASABI_CHECK_EQUAL(1,   cube.getBody().getValue(cube.getBody().getIndex("Meas0"), 0, 0).getDouble());
			WASABI_CHECK_EQUAL(3,   cube.getBody().getValue(cube.getBody().getIndex("Meas0"), 0, 3).getDouble());
			WASABI_CHECK_EQUAL("2", cube.getBody().getValue(cube.getBody().getIndex("Meas1"), 0, 0).getString());
			WASABI_CHECK_EQUAL("2", cube.getBody().getValue(cube.getBody().getIndex("Meas1"), 0, 3).getString());
	}

	JSONReader reader;
	{		
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

		WASABI_CHECK_EQUAL(4, cube.getAxe(calculator::eAxe::Row).getCardinality());
			WASABI_CHECK_EQUAL(1,    cube.getBody().getValue(cube.getBody().getIndex("Meas0"), 0, 0).getDouble());
			WASABI_CHECK_EQUAL(1+9,  cube.getBody().getValue(cube.getBody().getIndex("fct1"),  0, 0).getDouble());
			WASABI_CHECK_EQUAL(1+9+7,cube.getBody().getValue(cube.getBody().getIndex("fct2"),  0, 0).getDouble());
			WASABI_CHECK_EQUAL(2,    cube.getBody().getValue(cube.getBody().getIndex("Meas0"), 0, 1).getDouble());
			WASABI_CHECK_EQUAL(2+9,  cube.getBody().getValue(cube.getBody().getIndex("fct1"),  0, 1).getDouble());
			WASABI_CHECK_EQUAL(2+9+7,cube.getBody().getValue(cube.getBody().getIndex("fct2"),  0, 1).getDouble());
		WASABI_CHECK_EQUAL(0, cube.getAxe(calculator::eAxe::Column).getCardinality());
	}

	{
		ina::query_model::Selection selection;
		read(selection, reader.parse(R"({"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"FieldName":"[Measures].[Measures]","Elements":[{"Comparison":"=","Low":"Meas0"}]}},{"SetOperand":{"FieldName":"Dim","Elements":[{"Comparison":"=","Low":"A"}]}}]}})"));

		Cube cube;	
		cube.setStorage(storage);
		cube.addMeasure(Object("Meas0"));
		cube.addRestriction(Object("rest1"), selection);
		cube.materialyze();

		WASABI_CHECK_EQUAL(1.0, cube.getBody().getValue(cube.getBody().getIndex("rest1"), 0, 0).getDouble());
	}

	{
		ina::query_model::Selection selection;
		read(selection, reader.parse(R"({"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"FieldName":"[Measures].[Measures]","Elements":[{"Comparison":"=","Low":"Meas0"}]}},{"SetOperand":{"FieldName":"DimNum","Elements":[{"Comparison":"=","Low":1}]}}]}})"));

		Cube cube;	
		cube.setStorage(storage);
		cube.addMeasure(Object("Meas0"));
		cube.addRestriction(Object("rest1"), selection);
		cube.materialyze();

		WASABI_CHECK_EQUAL(2.0, cube.getBody().getValue(cube.getBody().getIndex("rest1"), 0, 0).getDouble());
	}

	{
		ina::query_model::Selection selection1;
		read(selection1, reader.parse(R"({"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"FieldName":"[Measures].[Measures]","Elements":[{"Comparison":"=","Low":"Meas0"}]}},{"SetOperand":{"FieldName":"Dim","Elements":[{"Comparison":"=","Low":"A"}]}}]}})"));

		ina::query_model::Selection selection2;
		read(selection2, reader.parse(R"({"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"FieldName":"[Measures].[Measures]","Elements":[{"Comparison":"=","Low":"Meas0"}]}},{"SetOperand":{"FieldName":"Dim","Elements":[{"Comparison":"=","Low":"B"}]}}]}})"));

		Cube cube;	
		cube.setStorage(storage);
		cube.addRestriction(Object("rest1"), selection1);
		cube.addRestriction(Object("rest2"), selection2);
		cube.materialyze();

		WASABI_CHECK_EQUAL(1.0, cube.getBody().getValue(cube.getBody().getIndex("rest1"), 0, 0).getDouble());
		WASABI_CHECK_EQUAL(2.0, cube.getBody().getValue(cube.getBody().getIndex("rest2"), 0, 0).getDouble());
	}

	return TEST_HAVEERROR();
}
