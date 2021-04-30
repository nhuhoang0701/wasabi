#include "query_generator.h"

#include <InA_query_model/Query.h>
#include <test_tools/TestAssert.h>
#include <json/jsonReader.h>
#include <iostream>

int main()
{
	TEST_INIT();

	{
		ina::query_model::DataSource ds;
		ds.setObjectName("MyTable");

		ina::query_model::Query queryInA;
		queryInA.setDataSource(ds);
		
		ina::query_model::Definition definition;
		queryInA.setDefinition(definition);

		const query_generator::query_generator& queryGen = query_generator::query_generator(queryInA);
		std::string sql = queryGen.getSQL();

		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("", sql);
	}
	{
		ina::query_model::DataSource ds;
		ds.setObjectName("MyTable");

		ina::query_model::Query queryInA;
		queryInA.setDataSource(ds);
		
		ina::query_model::Definition definition;
		definition.addDimension(ina::query_model::Dimension("dim_A", ina::query_model::Dimension::eAxe::Rows));
		
		definition.addDimension(ina::query_model::Dimension("dim_B", ina::query_model::Dimension::eAxe::Rows));
		
		ina::query_model::Dimension dimensionMeasure(ina::query_model::Dimension::DIMENSION_OF_MEASURES_NAME, ina::query_model::Dimension::eAxe::Rows);
		dimensionMeasure.addMember(ina::query_model::Member("meas_1", "SUM"));
		dimensionMeasure.addMember(ina::query_model::Member("meas_2", "SUM"));
		definition.addDimension(dimensionMeasure);

		queryInA.setDefinition(definition);

		const query_generator::query_generator& queryGen = query_generator::query_generator(queryInA);
		std::string sql = queryGen.getSQL();

		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT dim_A, dim_B, SUM(meas_1), SUM(meas_2) FROM MyTable GROUP BY dim_A, dim_B;", sql);
	}
	{
		ina::query_model::DataSource ds;
		ds.setObjectName("MyTable");

		ina::query_model::Query queryInA;
		queryInA.setDataSource(ds);

		std::string request = R"({"Dimensions":[{"Attributes":[{"Name":"OBJ_188","Obtainability":"UserInterface"}],"Axis":"Rows","Name":"OBJ_188","NonEmpty":true,"ReadMode":"Booked","ResultStructure":[{"Result":"Members","Visibility":"Visible"}]},{"Attributes":[{"Name":"[Measures].[Measures]","Obtainability":"UserInterface"},{"Name":"[Measures].[Name]","Obtainability":"UserInterface"}],"Axis":"Columns","Members":[{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_147"},"Visibility":"Visible"},{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_262"},"Visibility":"Visible"}],"Name":"CustomDimension1","NonEmpty":false,"ReadMode":"Master"}],"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"=","Low":"OBJ_262"}],"FieldName":"[Measures].[Measures]"}},{"SetOperand":{"Elements":[{"Comparison":"=","Low":"2014"}],"FieldName":"OBJ_188"}}]}}},"ResultSetFeatureRequest":{"ResultEncoding":"None","ResultFormat":"Version2","ReturnedDataSelection":{"Actions":false,"CellDataType":false,"CellFormat":false,"CellMeasure":false,"CellValueTypes":false,"ExceptionAlertLevel":false,"ExceptionName":false,"ExceptionSettings":false,"Exceptions":false,"InputEnabled":false,"NumericRounding":false,"NumericShift":false,"TupleDisplayLevel":false,"TupleDrillState":false,"TupleElementIds":true,"TupleElementIndexes":false,"TupleLevel":false,"TupleParentIndexes":false,"UnitDescriptions":false,"UnitTypes":false,"Units":false,"Values":false,"ValuesFormatted":false,"ValuesRounded":false},"SubSetDescription":{"ColumnFrom":0,"ColumnTo":60,"RowFrom":0,"RowTo":500},"UseDefaultAttributeKey":false},"Sort":[]})";
		JSONReader reader;
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);

		queryInA.setDefinition(definition);

		const query_generator::query_generator& queryGen = query_generator::query_generator(queryInA);
		std::string sql = queryGen.getSQL();
		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT OBJ_188, SUM(OBJ_262) FROM MyTable WHERE OBJ_188 = 2014 GROUP BY OBJ_188;", sql);
	}
	{
		ina::query_model::DataSource ds;
		ds.setObjectName("MyTable");

		ina::query_model::Query queryInA;
		queryInA.setDataSource(ds);

		std::string request = R"({"Dimensions":[{"Attributes":[{"Name":"OBJ_188","Obtainability":"UserInterface"}],"Axis":"Rows","Name":"OBJ_188","NonEmpty":true,"ReadMode":"Booked","ResultStructure":[{"Result":"Members","Visibility":"Visible"}]},{"Attributes":[{"Name":"[Measures].[Measures]","Obtainability":"UserInterface"},{"Name":"[Measures].[Name]","Obtainability":"UserInterface"}],"Axis":"Columns","Members":[{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_262"},"Visibility":"Visible"},{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_147"},"Visibility":"Visible"}],"Name":"CustomDimension1","NonEmpty":false,"ReadMode":"Master"}],"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"Operator":{"Code":"And","SubSelections":[{"Operator":{"Code":"Or","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"IS_NULL"}],"FieldName":"OBJ_188"}},{"SetOperand":{"Elements":[{"Comparison":"=","IsExcluding":true,"Low":"2014"}],"FieldName":"OBJ_188"}}]}}]}}]}}}})";
		JSONReader reader;
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);

		queryInA.setDefinition(definition);

		const query_generator::query_generator& queryGen = query_generator::query_generator(queryInA);
		std::string sql = queryGen.getSQL();
		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT OBJ_188 FROM MyTable WHERE OBJ_188 IS NULL OR NOT ( OBJ_188 = 2014 ) GROUP BY OBJ_188;", sql);
	}
	{
		ina::query_model::DataSource ds;
		ds.setObjectName("MyTable");

		ina::query_model::Query queryInA;
		queryInA.setDataSource(ds);

		std::string request = R"({"Dimensions":[{"Name":"Yr","Axis":"Rows"},{"Name":"Month_name","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"Sales_revenue", "Aggregation":"SUM"}]}]})";
		JSONReader reader;
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		std::cout << "Dimension size " << definition.getDimensions().size() << std::endl;
		CPPUNIT_ASSERT_EQUAL(definition.getDimensions().size(),3);
		queryInA.setDefinition(definition);
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryInA);
		std::string sql = queryGen.getSQL();
		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT Yr, Month_name, SUM(Sales_revenue) FROM MyTable GROUP BY Yr, Month_name;", sql);		
	}	
	{
		ina::query_model::DataSource ds;
		ds.setObjectName("MyTable");

		ina::query_model::Query queryInA;
		queryInA.setDataSource(ds);

		std::string request = R"({"Dimensions":[{"Attributes":[{"Name":"[Measures].[Measures]","Obtainability":"UserInterface"},{"Name":"[Measures].[Name]","Obtainability":"UserInterface"}],"Axis":"Columns","Members":[{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_147"},"Visibility":"Visible"},{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_191"},"Visibility":"Visible"}],"Name":"CustomDimension1","NonEmpty":false,"ReadMode":"Master"},{"Attributes":[{"Name":"OBJ_166","Obtainability":"UserInterface"}],"Axis":"Columns","Name":"OBJ_166","NonEmpty":false,"ReadMode":"Master","ResultStructure":[{"Result":"Members","Visibility":"Visible"}]},{"Attributes":[{"Name":"OBJ_185","Obtainability":"UserInterface"}],"Axis":"Columns","Name":"OBJ_185","NonEmpty":false,"ReadMode":"Master","ResultStructure":[{"Result":"Members","Visibility":"Visible"}]},{"Attributes":[{"Name":"OBJ_186","Obtainability":"UserInterface"}],"Axis":"Columns","Name":"OBJ_186","NonEmpty":false,"ReadMode":"Master","ResultStructure":[{"Result":"Members","Visibility":"Visible"}]}],"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"=","Low":"OBJ_147"}],"FieldName":"[Measures].[Measures]"}}]}}},"Sort":[{"Direction":"Asc","FieldName":"OBJ_166","PreserveGrouping":false,"SortType":"Field"},{"Direction":"Desc","FieldName":"OBJ_185","PreserveGrouping":false,"SortType":"Field"},{"Direction":"Asc","FieldName":"OBJ_186","PreserveGrouping":false,"SortType":"Field"}]})";
		JSONReader reader;
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		queryInA.setDefinition(definition);
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryInA);
		std::string sql = queryGen.getSQL();
		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT SUM(OBJ_147), OBJ_166, OBJ_185, OBJ_186 FROM MyTable GROUP BY OBJ_166, OBJ_185, OBJ_186 ORDER BY OBJ_166 ASC, OBJ_185 DESC, OBJ_186 ASC;", sql);		
	}
	{
		ina::query_model::DataSource ds;
		ds.setObjectName("MyTable");

		ina::query_model::Query queryInA;
		queryInA.setDataSource(ds);

		std::string request = R"({"Dimensions":[{"Name":"CustomDimension1","Axis":"Columns","Members":[{"MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"Sales_revenue"},"Aggregation":"SUM","Visibility":"Visible"},{"MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"Quantity_sold"},"Aggregation":"SUM","Visibility":"Visible"},{"MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"Margin"},"Aggregation":"SUM","Visibility":"Visible"}],"Attributes":[{"Name":"[Measures].[Name]","Obtainability":"Always"},{"Name":"[Measures].[Measures]","Obtainability":"UserInterface"}]}],"Sort":[{"SortType":"MemberKey","PreserveGrouping":true,"Dimension":"CustomDimension1"}]})";
		JSONReader reader;
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		queryInA.setDefinition(definition);
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryInA);
		std::string sql = queryGen.getSQL();
		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT SUM(Sales_revenue), SUM(Quantity_sold), SUM(Margin) FROM MyTable;", sql);		

	}	
	return TEST_HAVEERROR();
}
