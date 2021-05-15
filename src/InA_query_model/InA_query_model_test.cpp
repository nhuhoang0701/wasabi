#include "InA_query_model/Dimension.h"
#include "InA_query_model/Formula.h"
#include "InA_query_model/Member.h"
#include "InA_query_model/Parameter.h"
#include "InA_query_model/QuerySort.h"
#include "Query.h"
#include "Definition.h"
#include "DataSource.h"
#include <json/jsonReader.h>
#include <test_tools/TestAssert.h>

#include <iostream>
#include <sstream>


int main()
{
	TEST_INIT();
	JSONReader reader;
	{
		ina::query_model::Query query;

		ina::query_model::DataSource ds;
		ds.setObjectName("MyTable");
		
		ina::query_model::Definition definition;

		definition.addDimension(ina::query_model::Dimension("Dim_A", ina::query_model::Dimension::eAxe::Columns));
		definition.addDimension(ina::query_model::Dimension("Dim_B", ina::query_model::Dimension::eAxe::Rows));
		
		ina::query_model::Dimension dimensionMeasure("Meas_1", ina::query_model::Dimension::eAxe::Rows);
		ina::query_model::Member member;
		read(member, reader.parse(R"({"Description":"Meas 1","Name":"Meas_1", "Aggregation":"SUM"})"));
		dimensionMeasure.addMember(member);
		definition.addDimension(dimensionMeasure);

		CPPUNIT_ASSERT_EQUAL("MyTable", ds.getObjectName());
		CPPUNIT_ASSERT_EQUAL(3, definition.getDimensions().size());
		CPPUNIT_ASSERT_EQUAL("Dim_A", definition.getDimensions()[0].getName());
		CPPUNIT_ASSERT_EQUAL("SUM", definition.getDimensions()[2].getMembers()[0].getAggregation());
	}

	{
		std::string request = R"({"Name":"dimName","Axis":"Rows","Attributes":[{"Name":"ATTR0"}, {"Name":"ATTR1"}]})";
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Dimension dimension;
		read(dimension, root);

		CPPUNIT_ASSERT_EQUAL(2, dimension.getAttributes().size());
		CPPUNIT_ASSERT_EQUAL_STR("ATTR0", dimension.getAttributes().at(0).getName().c_str());
		CPPUNIT_ASSERT_EQUAL_STR("ATTR1", dimension.getAttributes().at(1).getName().c_str());
    }
	{
		std::string request = R"({"Dimensions":[{"Name":"text","Axis":"Rows"},{"Name":"varchar","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"real", "Aggregation":"SUM"}]}]})";
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		CPPUNIT_ASSERT_EQUAL(3, definition.getDimensions().size());
	}
	{
		std::string request = R"({"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"=","Low":"OBJ_147"},{"Comparison":"<>","Low":"OBJ_191"}],"FieldName":"[Measures].[Measures]"}}]}}}})";
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		CPPUNIT_ASSERT_EQUAL(2, definition.getQueryFilters().size());
		
		CPPUNIT_ASSERT_EQUAL("[Measures].[Measures]", definition.getQueryFilters().at(0).getFieldName());
		CPPUNIT_ASSERT_EQUAL(ina::query_model::QueryFilter::ComparisonOperator::EqualTo, definition.getQueryFilters().at(0).getComparisonOperator());
		CPPUNIT_ASSERT_EQUAL("OBJ_147", definition.getQueryFilters().at(0).getLowValue());
		
		CPPUNIT_ASSERT_EQUAL("[Measures].[Measures]", definition.getQueryFilters().at(1).getFieldName());
		CPPUNIT_ASSERT_EQUAL(ina::query_model::QueryFilter::ComparisonOperator::NotEqualTo, definition.getQueryFilters().at(1).getComparisonOperator());
		CPPUNIT_ASSERT_EQUAL("OBJ_191", definition.getQueryFilters().at(1).getLowValue());

		//TODO: check with attributes KEY
	}
	{
		std::string request = R"({"Dimensions":[{"Members":[{"Description":"Calculated Measure 1","Formula":{"Function":{"Name":"**","Parameters":[{"Member":{"Name":"OBJ_147"}},{"Function":{"Name":"decfloat","Parameters":[{"Constant":{"Value":"2","ValueType":"String"}}]}}]}},"Name":"32160367-6930-4537-9181-755582731239"}],"Axis":"Rows","Name":"CustomDimension1"}]})";;
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		CPPUNIT_ASSERT_EQUAL(1, definition.getDimensions().size());
		CPPUNIT_ASSERT_EQUAL(1, definition.getDimensions().at(0).getMembers().size());
	}
	{
		std::string request = R"({"Dimensions":[{"Attributes":[{"Name":"OBJ_188"}],"Axis":"Rows","Name":"OBJ_188"},{"Attributes":[{"Name":"[Measures].[Measures]"},{"Name":"[Measures].[Name]"}],"Axis":"Columns","Members":[{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_147"}},{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_262"}}],"Name":"CustomDimension1"}],"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"=","Low":"OBJ_262"}],"FieldName":"[Measures].[Measures]"}},{"SetOperand":{"Elements":[{"Comparison":"=","Low":"2014"}],"FieldName":"OBJ_188"}}]}}},"Sort":[]})";
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		auto dimensionYear 		= definition.getDimensions().at(0);
		auto dimensionMeasure 	= definition.getDimensions().at(1);
		CPPUNIT_ASSERT_EQUAL(2, definition.getDimensions().size());
		CPPUNIT_ASSERT_EQUAL(2, definition.getQueryFilters().size());

		CPPUNIT_ASSERT_EQUAL(0, dimensionYear.getMembers().size());
		CPPUNIT_ASSERT_EQUAL(0, definition.getVisibleMembers(dimensionYear).size());		
		CPPUNIT_ASSERT_EQUAL(1, dimensionYear.getAttributes().size());

		auto attributeYear = dimensionYear.getAttributes().at(0);
		CPPUNIT_ASSERT_EQUAL("OBJ_188", attributeYear.getName());

		CPPUNIT_ASSERT_EQUAL(2, dimensionMeasure.getMembers().size());
		CPPUNIT_ASSERT_EQUAL(1, definition.getVisibleMembers(dimensionMeasure).size());
		CPPUNIT_ASSERT(dimensionMeasure.getMembers().at(0).getMemberOperand() != nullptr);
		CPPUNIT_ASSERT(dimensionMeasure.getMembers().at(0).getFormula() == nullptr);
		CPPUNIT_ASSERT_EQUAL("OBJ_147", dimensionMeasure.getMembers().at(0).getMemberOperand()->getValue());
	}
	{
		std::string request = R"({"Sort":[{"Dimension":"Week ?","Direction":"None","SortType":"MemberKey"}]})";
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		CPPUNIT_ASSERT_EQUAL(1, definition.getQuerySorts().size());
		CPPUNIT_ASSERT_EQUAL("Week ?", definition.getQuerySorts().at(0).getObjectName());
		CPPUNIT_ASSERT_EQUAL(ina::query_model::QuerySort::SortType::MemberKey, definition.getQuerySorts().at(0).getSortType());
		CPPUNIT_ASSERT_EQUAL(ina::query_model::QuerySort::Direction::None, definition.getQuerySorts().at(0).getDirection());
	}
	{
		std::string request = R"({"Dimensions": [{"Members": [{"Description": "Calculated Measure 1","Formula": {"Function": {"Name": "+","Parameters": [{"Member": {"Name": "OBJ_147"}},{"Function": {"Name": "decfloat","Parameters": [{"Constant": {"Value": "1","ValueType": "String"}}]}}]}},"Name": "70027803-5182-4685-b851-864623689423","NumericScale": 7,"Visibility": "Visible"}],"Axis": "Columns","Name": "CustomDimension1"}]})";
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		auto dimension 		= definition.getDimensions().at(0);
		CPPUNIT_ASSERT_EQUAL(1, definition.getDimensions().size());

		CPPUNIT_ASSERT_EQUAL(1, dimension.getMembers().size())
		CPPUNIT_ASSERT(dimension.getMembers().at(0).getFormula() != nullptr)

		auto member = dimension.getMembers().at(0);
		
		CPPUNIT_ASSERT_EQUAL(2, member.getFormula()->getFunction().getChildrenCount());

		auto param1  =member.getFormula()->getFunction().getChild(0);
		auto param2  =member.getFormula()->getFunction().getChild(1);
		CPPUNIT_ASSERT_EQUAL(ina::query_model::Parameter::eMember, param1.getType());
		CPPUNIT_ASSERT_EQUAL(ina::query_model::Parameter::eFunction, param2.getType());

		auto func = param2.getFunction();
		CPPUNIT_ASSERT_EQUAL(ina::query_model::Function::eDecFloat, func.getFunctionType());

		CPPUNIT_ASSERT_EQUAL(1, func.getChildrenCount());

		auto param = func.getChild(0);
		CPPUNIT_ASSERT_EQUAL(ina::query_model::Parameter::eConstant, param.getType());
		
	}
	{
		std::string request = R"({"ResultSetFeatureRequest":{"SubSetDescription":{"RowFrom":1,"RowTo":2,"ColumnFrom":3,"ColumnTo":4}}})";
		JSONGenericObject root = reader.parse(request);
		ina::query_model::Definition definition;
		read(definition, root);
		CPPUNIT_ASSERT_EQUAL(3, definition.getResultSetFeat().getSubSetDescription().m_ColumnFrom);
		CPPUNIT_ASSERT_EQUAL(4, definition.getResultSetFeat().getSubSetDescription().m_ColumnTo);
		CPPUNIT_ASSERT_EQUAL(1, definition.getResultSetFeat().getSubSetDescription().m_RowFrom);
		CPPUNIT_ASSERT_EQUAL(2, definition.getResultSetFeat().getSubSetDescription().m_RowTo);
	}

	return TEST_HAVEERROR();
}
