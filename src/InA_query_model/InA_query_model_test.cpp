#include "Dimension.h"
#include "Member.h"
#include "Formula.h"
#include "Function.h"
#include "Parameter.h"
#include "Sort.h"
#include "Selection/SelectionElement.h"
#include "Selection/Element.h"
#include "Query.h"
#include "QueryEx.h"
#include "Definition.h"
#include "DataSource.h"

#include <json/jsonReader.h>
#include <test_tools/TestAssert.h>

#include <iterator>
#include <ostream>
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

		WASABI_CHECK_EQUAL("MyTable", ds.getObjectName());
		WASABI_CHECK_EQUAL(3, definition.getDimensions().size());
		WASABI_CHECK_EQUAL("Dim_A", definition.getDimensions()[0].getName());
		WASABI_CHECK_EQUAL("SUM", definition.getDimensions()[2].getMembers()[0].getAggregation());
	}

	{
		ina::query_model::Dimension dimension;
		read(dimension, reader.parse(R"({"Name":"dimName","Axis":"Rows","Attributes":[{"Name":"ATTR0"}, {"Name":"ATTR1"}]})"));

		WASABI_CHECK_EQUAL(2, dimension.getAttributes().size());
		CPPUNIT_ASSERT_EQUAL_STR("ATTR0", dimension.getAttributes().at(0).getName().c_str());
		CPPUNIT_ASSERT_EQUAL_STR("ATTR1", dimension.getAttributes().at(1).getName().c_str());
    }
	{
		std::string request = R"({"Dimensions":[{"Name":"text","Axis":"Rows"},{"Name":"varchar","Axis":"Rows"},{"Name":"CustomDimension1","Axis":"Columns","Members":[{"Description":"Measure 1","Name":"real", "Aggregation":"SUM"}]}]})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));
		WASABI_CHECK_EQUAL(3, definition.getDimensions().size());
	}
	{
		std::string request = R"({"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"=","Low":"OBJ_147"},{"Comparison":"<>","Low":"OBJ_191"}],"FieldName":"[Measures].[Measures]"}}]}}}})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));
		C_WASABI_CHECK_EQUAL(ina::query_model::SelectionElement::Type::Operator, definition.getSelection().getOperator().getType());
		WASABI_CHECK_EQUAL(1, definition.getSelection().getOperator().getSubSelections().size());
		C_WASABI_CHECK_EQUAL(ina::query_model::SelectionElement::Type::SetOperand, definition.getSelection().getOperator().getSubSelections().at(0).getType());
		WASABI_CHECK_EQUAL(2, definition.getSelection().getOperator().getSubSelections().at(0).getElements().size());
		
		const ina::query_model::SelectionElement selectionElement = definition.getSelection().getOperator() .getSubSelections().at(0);
		std::cout << "[GetType]" << std::endl;
		C_WASABI_CHECK_EQUAL(ina::query_model::SelectionElement::Type::SetOperand, selectionElement.getType());
		WASABI_CHECK_EQUAL(2, selectionElement.getElements().size());
		WASABI_CHECK_EQUAL("[Measures].[Measures]", selectionElement.getFieldName());
		std::cout << "[setOperand1->getElements().at(0)]" << std::endl;
		ina::query_model::Element filter0 = selectionElement.getElements().at(0);
		C_WASABI_CHECK_EQUAL(ina::query_model::Element::ComparisonOperator::EqualTo, filter0.getComparisonOperator());
		WASABI_CHECK_EQUAL("OBJ_147", filter0.getLowValue().getString());
		ina::query_model::Element filter1 = selectionElement.getElements().at(1);
		C_WASABI_CHECK_EQUAL(ina::query_model::Element::ComparisonOperator::NotEqualTo, filter1.getComparisonOperator());
		WASABI_CHECK_EQUAL("OBJ_191", filter1.getLowValue().getString());

		//TODO: check with attributes KEY
	}

	{
		std::string request = R"({"DynamicFilter":{"Selection":{"SetOperand":{"FieldName":"Yr","Elements":[{"Comparison":"=","Low":"2016"}]}}}})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));

	}
	{
		std::string request = R"({"Dimensions":[{"Members":[{"Description":"Calculated Measure 1","Formula":{"Function":{"Name":"**","Parameters":[{"Member":{"Name":"OBJ_147"}},{"Function":{"Name":"decfloat","Parameters":[{"Constant":{"Value":"2","ValueType":"String"}}]}}]}},"Name":"32160367-6930-4537-9181-755582731239"}],"Axis":"Rows","Name":"CustomDimension1"}]})";;
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));
		WASABI_CHECK_EQUAL(1, definition.getDimensions().size());
		WASABI_CHECK_EQUAL(1, definition.getDimensions().at(0).getMembers().size());
	}
	{
		std::string request = R"({"Dimensions":[{"Attributes":[{"Name":"OBJ_188"}],"Axis":"Rows","Name":"OBJ_188"},{"Attributes":[{"Name":"[Measures].[Measures]"},{"Name":"[Measures].[Name]"}],"Axis":"Columns","Members":[{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_147"}},{"Aggregation":"SUM","MemberOperand":{"AttributeName":"Measures","Comparison":"=","Value":"OBJ_262"}}],"Name":"CustomDimension1"}],"DynamicFilter":{"Selection":{"Operator":{"Code":"And","SubSelections":[{"SetOperand":{"Elements":[{"Comparison":"=","Low":"OBJ_262"}],"FieldName":"[Measures].[Measures]"}},{"SetOperand":{"Elements":[{"Comparison":"=","Low":"2014"}],"FieldName":"OBJ_188"}}]}}},"Sort":[]})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));

		ina::query_model::QueryEx queryEx(definition);

		auto dimensionYear 		= definition.getDimensions().at(0);
		auto dimensionMeasure 	= definition.getDimensions().at(1);
		WASABI_CHECK_EQUAL(2, definition.getDimensions().size());
		const ina::query_model::SelectionElement selectionOperator1 = definition.getSelection().getOperator();
		const ina::query_model::SelectionElement setOperand1 = (selectionOperator1.getSubSelections().at(0));
		const ina::query_model::SelectionElement setOperand2 = (selectionOperator1.getSubSelections().at(1));

		WASABI_CHECK_EQUAL(1, setOperand1.getElements().size());
		WASABI_CHECK_EQUAL(1, setOperand2.getElements().size());

		WASABI_CHECK_EQUAL(0, dimensionYear.getMembers().size());
		WASABI_CHECK_EQUAL(0, queryEx.getVisibleMembers(dimensionYear).size());		
		WASABI_CHECK_EQUAL(1, dimensionYear.getAttributes().size());

		auto attributeYear = dimensionYear.getAttributes().at(0);
		WASABI_CHECK_EQUAL("OBJ_188", attributeYear.getName());

		WASABI_CHECK_EQUAL(2, dimensionMeasure.getMembers().size());
		WASABI_CHECK_EQUAL(1, queryEx.getVisibleMembers(dimensionMeasure).size());
		WASABI_CHECK_NOTEQUAL(dimensionMeasure.getMembers().at(0).getMemberOperand(), nullptr);
		WASABI_CHECK_EQUAL(dimensionMeasure.getMembers().at(0).getFormula(), nullptr);
		WASABI_CHECK_EQUAL("OBJ_147", dimensionMeasure.getMembers().at(0).getMemberOperand()->getValue());
	}
	{
		std::string request = R"({"Sort":[{"Dimension":"Week ?","Direction":"None","SortType":"MemberKey"}]})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));
		WASABI_CHECK_EQUAL(1, definition.getSorts().size());
		WASABI_CHECK_EQUAL("Week ?", definition.getSorts().at(0).getObjectName());
		C_WASABI_CHECK_EQUAL(ina::query_model::Sort::SortType::MemberKey, definition.getSorts().at(0).getSortType());
		C_WASABI_CHECK_EQUAL(ina::query_model::Sort::Direction::None, definition.getSorts().at(0).getDirection());
	}
	{
		std::string request = R"({"Dimensions": [{"Members": [{"Description": "Calculated Measure 1","Formula": {"Function": {"Name": "+","Parameters": [{"Member": {"Name": "OBJ_147"}},{"Function": {"Name": "decfloat","Parameters": [{"Constant": {"Value": "1","ValueType": "String"}}]}}]}},"Name": "70027803-5182-4685-b851-864623689423","NumericScale": 7,"Visibility": "Visible"}],"Axis": "Columns","Name": "CustomDimension1"}]})";
		ina::query_model::Definition definition;
		read(definition, reader.parse(request));
		auto dimension 		= definition.getDimensions().at(0);
		WASABI_CHECK_EQUAL(1, definition.getDimensions().size());

		WASABI_CHECK_EQUAL(1, dimension.getMembers().size())
		WASABI_CHECK_NOTEQUAL(dimension.getMembers().at(0).getFormula(), nullptr)

		auto member = dimension.getMembers().at(0);
		
		WASABI_CHECK_EQUAL(ina::query_model::Parameter::eFunction, member.getFormula()->getParameter().getType());
		WASABI_CHECK_EQUAL(2, member.getFormula()->getParameter().getFunction().getParameterCount());

		auto param1  =member.getFormula()->getParameter().getFunction().getParameter(0);
		auto param2  =member.getFormula()->getParameter().getFunction().getParameter(1);
		WASABI_CHECK_EQUAL(ina::query_model::Parameter::eMember, param1.getType());
		WASABI_CHECK_EQUAL(ina::query_model::Parameter::eFunction, param2.getType());

		auto func = param2.getFunction();
		WASABI_CHECK_EQUAL(ina::query_model::Function::eDecFloat, func.getFunctionType());

		WASABI_CHECK_EQUAL(1, func.getParameterCount());

		auto param = func.getParameter(0);
		C_WASABI_CHECK_EQUAL(ina::query_model::Parameter::eConstant, param.getType());
		
	}
	{
		ina::query_model::Formula formula;
		read(formula, reader.parse(R"({"Name":"fct1", "Function":{"Name": "+","Parameters": [{"Constant": {"ValueType": "String","Value": "1"}},{"Function": {"Name": "*","Parameters": [{"Constant": {"ValueType": "String","Value": "2"}},{"Function": {"Name": "/","Parameters": [{"Function": {"Name": "+","Parameters": [{"Constant": {"ValueType": "String","Value": "3"}},{"Constant": {"ValueType": "String","Value": "4"}}]}},{"Constant": {"ValueType": "String","Value": "5"}}]}}]}}]}})"));

		WASABI_CHECK_EQUAL(3.8, eval(nullptr, formula, nullptr).getDouble());
	}
	{
		ina::query_model::Definition definition;
		read(definition, reader.parse( R"({"ResultSetFeatureRequest":{"SubSetDescription":{"RowFrom":1,"RowTo":2,"ColumnFrom":3,"ColumnTo":4}}})"));
		WASABI_CHECK_EQUAL(3, definition.getResultSetFeat().getSubSetDescription().m_ColumnFrom);
		WASABI_CHECK_EQUAL(4, definition.getResultSetFeat().getSubSetDescription().m_ColumnTo);
		WASABI_CHECK_EQUAL(1, definition.getResultSetFeat().getSubSetDescription().m_RowFrom);
		WASABI_CHECK_EQUAL(2, definition.getResultSetFeat().getSubSetDescription().m_RowTo);
	}

	return TEST_HAVEERROR();
}
