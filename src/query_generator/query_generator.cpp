#include "query_generator.h"

#include <calculator/storage.h>
#include <InA_metadata/Cube.h>
#include <InA_query_model/Query.h>
#include <InA_query_model/QueryEx.h>
#include <InA_query_model/Dimension.h>
#include <InA_query_model/Selection/Element.h>
#include <InA_query_model/Selection/SelectionElement.h>
#include <InA_query_model/Formula.h>

#include "InA_query_model/Sort.h"
#include "ModelSQLGenerator.h"

#include <ostream>
#include <sstream>

#include <stdexcept>
#include <string_view>
#include <utility>
#include <algorithm>


namespace query_generator
{
	std::string_view toString(ina::query_model::Sort::Direction direction)
	{
		if (direction == ina::query_model::Sort::Direction::Ascending || direction == ina::query_model::Sort::Direction::Default)
			return "ASC";
		else if (direction == ina::query_model::Sort::Direction::Descending)
			return "DESC";
		
		throw std::runtime_error("Unknow sort direction");
	}

	query_generator::query_generator(const ina::query_model::QueryEx& queryExec)
	 : m_queryExec(queryExec)
	{
	}

    std::string query_generator::getSQL() const
    {
		std::vector<std::tuple<std::string /*name*/,std::string/*aggregation*/, common::eDataType>> resultObjects;
		std::ostringstream where;
        std::vector<std::string> group_by;
		std::vector<std::pair<std::string /*name*/,ina::query_model::Sort::Direction>> order_by;

		// SELECT objects
		{
			m_queryExec.getResultObjects(resultObjects);

			if(m_dataStorage)
			{
				// Integrity check index beetwen query and data storage columns
				size_t idxInData = 0;
				for(const auto& resultObject : resultObjects)
				{
					if(m_dataStorage->getColIndex(std::get<0>(resultObject)) != idxInData)
						throw std::runtime_error("Missmatch col. index in data with query");
					idxInData++;
				}
			}
		}

        std::ostringstream sql;
		if(!resultObjects.empty())
		{
			m_queryExec.getGroupBy(group_by);
			
			const auto& selectionOperator = m_queryExec.getQueryDefinition().getSelection().getOperator();
			buildWhereClause(selectionOperator, where);

			m_queryExec.getSortObjects(order_by);

			// Generated statement
			std::string delim;
        	sql << "SELECT ";
			delim.clear();
			for(const auto& resultObject : resultObjects)
			{
				if(std::get<1>(resultObject).empty())
	        		sql << delim << std::get<0>(resultObject);
				else
					sql << delim << std::get<1>(resultObject) << "(" << std::get<0>(resultObject) << ")";
				if(delim.empty()) delim = ", ";
			}

			const std::string table = m_queryExec.getDSCube()==nullptr?"FAKE_TABLE_NO_DS":m_queryExec.getDSCube()->getDataSource().getObjectName();
			sql << " FROM " << table;
			if (!where.str().empty())
			{
				sql << where.str();
			}
			if(!group_by.empty())
			{
				sql << " GROUP BY ";
				delim.clear();
				for(const auto& s : group_by)
				{
					sql << delim << s;
					if(delim.empty()) delim = ", ";
				}
			}
			if(!order_by.empty())
			{
				sql << " ORDER BY ";
				delim.clear();
				for(const auto& s : order_by)
				{
					sql << delim << s.first;
					if(s.second != ina::query_model::Sort::Direction::Default && s.second != ina::query_model::Sort::Direction::None)
						sql  << " " << toString(s.second);
					if(delim.empty()) delim = ", ";
				}
			}

			sql << ";";
		}
        return sql.str();
    }

	void query_generator::buildWhereSetOperandClause(const ina::query_model::SelectionElement& setOperand, ina::query_model::LogicalOperator parentLogicalOperator, std::ostringstream& where) const
	{
		std::string SELECTIONS;
		bool first = true;
		const std::string& fieldName = setOperand.getFieldName();
		if ("[Measures].[Measures]" == fieldName)
			return;

		for(const auto& element : setOperand.getElements())
		{
			if(first == false)
				SELECTIONS += " OR ";
			SELECTIONS += generateSQL(fieldName, element);
			first = false;
		}

		if(SELECTIONS.empty() == false)
		{
			if (where.str().empty())
			{
				where << " WHERE ";
			}
			else
			{			
				std::string code = ina::query_model::toString(parentLogicalOperator);				
				where << " " << code << " ";
			}
			where << SELECTIONS;
		}
	}

	void query_generator::buildWhereClause(const ina::query_model::SelectionElement& selectionElement, std::ostringstream& where) const
	{
		if (selectionElement.getType() == ina::query_model::SelectionElement::Type::SetOperand)
		{
			buildWhereSetOperandClause(selectionElement, ina::query_model::LogicalOperator::Undefined, where);
		}
		else if (selectionElement.getType() == ina::query_model::SelectionElement::Type::Operator)
		{
			for(const auto& subSelectionElement : selectionElement.getSubSelections())
			{				
				if (subSelectionElement.getType() == ina::query_model::SelectionElement::Type::Operator)
				{
					buildWhereClause(subSelectionElement, where);
				}
				else if (subSelectionElement.getType() == ina::query_model::SelectionElement::Type::SetOperand)
				{
					buildWhereSetOperandClause(subSelectionElement, selectionElement.getCode(), where);
				}
			}
		}
	}
	
	void query_generator::prepareStorage(std::shared_ptr<calculator::DataStorage> data) const
	{
		if(data.get() == nullptr)
			throw std::runtime_error("query_generator::prepareStorage datastorage should not be null");

		std::vector<std::tuple<std::string /*name*/,std::string/*aggregation*/, common::eDataType>> resultObjects;
		m_queryExec.getResultObjects(resultObjects);
		for(const auto& resultObject: resultObjects)
			data->addColumn(std::get<0>(resultObject),std::get<2>(resultObject), std::get<1>(resultObject).empty()?calculator::eColumnType::Indexed:calculator::eColumnType::NoneIndexed);

		m_dataStorage = data;
	}
} //query_generator
