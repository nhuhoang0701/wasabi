#include "query_generator.h"

#include <InA_query_model/Query.h>
#include <InA_query_model/QueryEx.h>
#include <test_tools/TestAssert.h>
#include <json/jsonReader.h>
#include <iostream>

int main()
{
	TEST_INIT();
	
	JSONReader reader;	
	{
		ina::query_model::Definition definition;

		ina::query_model::QueryEx queryEx(definition);
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryEx);
		std::string sql = queryGen.getSQL();

		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("", sql);
	}
	{		
		ina::query_model::Definition definition;
		
		ina::query_model::Dimension dim_A;
		read(dim_A, reader.parse(R"({"Name":"dim_A","Axis":"Rows","Attributes":[{"Name":"dim_A"}]})"));
		definition.addDimension(dim_A);
		ina::query_model::Dimension dim_B;
		read(dim_B, reader.parse(R"({"Name":"dim_B","Axis":"Rows","Attributes":[{"Name":"dim_B"}]})"));
		definition.addDimension(dim_B);
		
		ina::query_model::Dimension dimensionMeasure(ina::query_model::QueryEx::DIMENSION_OF_MEASURES_NAME, ina::query_model::Dimension::eAxe::Rows);
		ina::query_model::Member member1;
		read(member1, reader.parse(R"({"Description":"meas 1","Name":"meas_1", "Aggregation":"SUM"})"));
		dimensionMeasure.addMember(member1);
		ina::query_model::Member member2;
		read(member2, reader.parse(R"({"Description":"meas 2","Name":"meas_2", "Aggregation":"SUM"})"));
		dimensionMeasure.addMember(member2);
		definition.addDimension(dimensionMeasure);

		ina::query_model::QueryEx queryEx(definition);
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryEx);
		std::string sql = queryGen.getSQL();

		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT dim_A, dim_B, SUM(meas_1), SUM(meas_2) FROM FAKE_TABLE_NO_DS GROUP BY dim_A, dim_B;", sql);
	}
	{
		std::string request = R"({"Dimensions":[{"Attributes":[{"Name":"OBJ_188"}],"Axis":"Rows","Name":"OBJ_188"},{"Attributes":[{"Name":"[Measures].[Measures]"},{"Name":"[Measures].[Name]"}],"Axis":"Columns","Members":[{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_147"}},{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_262"}}],"Name":"CustomDimension1"}],"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"=","Low":"OBJ_262"}],"FieldName":"[Measures].[Measures]"}},{"SetOperand":{"Elements":[{"Comparison":"=","Low":"2014"}],"FieldName":"OBJ_188"}}]}}},"Sort":[]})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));

		ina::query_model::QueryEx queryEx(definition);
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryEx);
		std::string sql = queryGen.getSQL();
		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT OBJ_188, SUM(OBJ_262) FROM FAKE_TABLE_NO_DS WHERE OBJ_188 = '2014' GROUP BY OBJ_188;", sql);
	}	
	{
		std::string request = R"({"Dimensions":[{"Attributes":[{"Name":"OBJ_188"}],"Axis":"Rows","Name":"OBJ_188"},{"Attributes":[{"Name":"[Measures].[Measures]"},{"Name":"[Measures].[Name]"}],"Axis":"Columns","Members":[{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_262"}},{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_147"}}],"Name":"CustomDimension1"}],"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"Operator":{"Code":"And","SubSelections":[{"Operator":{"Code":"Or","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"IS_NULL"}],"FieldName":"OBJ_188"}},{"SetOperand":{"Elements":[{"Comparison":"=","IsExcluding":true,"Low":"2014"}],"FieldName":"OBJ_188"}}]}}]}}]}}}})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));

		ina::query_model::QueryEx queryEx(definition);
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryEx);
		std::string sql = queryGen.getSQL();
		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT OBJ_188, SUM(OBJ_262), SUM(OBJ_147) FROM FAKE_TABLE_NO_DS WHERE OBJ_188 IS NULL OR NOT ( OBJ_188 = '2014' ) GROUP BY OBJ_188;", sql);
	}	
	{
		std::string request = R"({"Dimensions":[{"Name":"Yr","Axis":"Rows","Attributes":[{"Name": "Yr"}]},{"Name":"Month_name","Axis":"Rows","Attributes":[{"Name": "Month_name"}]},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"Sales_revenue", "Aggregation":"SUM"}]}]})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));
		std::cout << "Dimension size " << definition.getDimensions().size() << std::endl;
		CPPUNIT_ASSERT_EQUAL(definition.getDimensions().size(),3);
		
		ina::query_model::QueryEx queryEx(definition);
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryEx);
		std::string sql = queryGen.getSQL();
		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT Yr, Month_name, SUM(Sales_revenue) FROM FAKE_TABLE_NO_DS GROUP BY Yr, Month_name;", sql);		
	}	
	{
		std::string request = R"({"Dimensions":[{"Attributes":[{"Name":"[Measures].[Measures]"},{"Name":"[Measures].[Name]"}],"Axis":"Columns","Members":[{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_147"}},{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_191"}}],"Name":"CustomDimension1"},{"Attributes":[{"Name":"OBJ_166"}],"Axis":"Columns","Name":"OBJ_166"},{"Attributes":[{"Name":"OBJ_185"}],"Axis":"Columns","Name":"OBJ_185"},{"Attributes":[{"Name":"OBJ_186"}],"Axis":"Columns","Name":"OBJ_186"}],"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"=","Low":"OBJ_147"}],"FieldName":"[Measures].[Measures]"}}]}}},"Sort":[{"Direction":"Asc","FieldName":"OBJ_166","PreserveGrouping":false,"SortType":"Field"},{"Direction":"Desc","FieldName":"OBJ_185","PreserveGrouping":false,"SortType":"Field"},{"Direction":"Asc","FieldName":"OBJ_186","PreserveGrouping":false,"SortType":"Field"}]})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));

		ina::query_model::QueryEx queryEx(definition);
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryEx);
		std::string sql = queryGen.getSQL();
		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT SUM(OBJ_147), OBJ_166, OBJ_185, OBJ_186 FROM FAKE_TABLE_NO_DS GROUP BY OBJ_166, OBJ_185, OBJ_186 ORDER BY OBJ_166 ASC, OBJ_185 DESC, OBJ_186 ASC;", sql);		
	}
	{
		std::string request = R"({"Dimensions":[{"Name":"CustomDimension1","Axis":"Columns","Members":[{"MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"Sales_revenue"},"Aggregation":"SUM"},{"MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"Quantity_sold"},"Aggregation":"SUM"},{"MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"Margin"},"Aggregation":"SUM"}],"Attributes":[{"Name":"[Measures].[Name]","Obtainability":"Always"},{"Name":"[Measures].[Measures]"}]}],"Sort":[{"SortType":"MemberKey","PreserveGrouping":true,"Dimension":"CustomDimension1"}]})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));

		ina::query_model::QueryEx queryEx(definition);
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryEx);
		std::string sql = queryGen.getSQL();
		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT SUM(Sales_revenue), SUM(Quantity_sold), SUM(Margin) FROM FAKE_TABLE_NO_DS;", sql);		

	}	
	{
		std::string request = R"({"Dimensions":[{"Name":"Yr","Axis":"Rows","Attributes":[{"Name":"Yr","Obtainability":"UserInterface"}]},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"Sales_revenue"}},{"MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"Quantity_sold"}},{"MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"Margin"}}],"Attributes":[{"Name":"[Measures].[Measures]","Obtainability":"UserInterface"},{"Name":"[Measures].[Name]","Obtainability":"UserInterface"}]}],"Sort":[{"SortType":"MemberKey","PreserveGrouping":true,"Dimension":"Yr"},{"SortType":"MemberKey","PreserveGrouping":true,"Dimension":"CustomDimension1"}],"DynamicFilter":{"Selection":{"SetOperand":{"FieldName":"[Measures].[Measures]","Elements":[{"Comparison":"=","Low":"Quantity_sold"}]}}}})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));

		ina::query_model::QueryEx queryEx(definition);
		const query_generator::query_generator& queryGen = query_generator::query_generator(queryEx);
		std::string sql = queryGen.getSQL();
		std::cout << "Generated SQL: " << sql << std::endl;
		CPPUNIT_ASSERT_EQUAL("SELECT Yr, SUM(Quantity_sold) FROM FAKE_TABLE_NO_DS GROUP BY Yr ORDER BY Yr;", sql);		

	}
	
	return TEST_HAVEERROR();
}
