#include "query_generator.h"

#include <calculator/storage.h>
#include <InA_metadata/Cube.h>
#include <InA_query_model/Query.h>
#include <InA_query_model/QueryEx.h>
#include <InA_query_model/Dimension.h>
#include <InA_query_model/Selection/Element.h>
#include <InA_query_model/Selection/SelectionElement.h>
#include <InA_query_model/Formula.h>

#include "ModelSQLGenerator.h"

#include <ostream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <algorithm>


namespace query_generator
{
	query_generator::query_generator(const ina::query_model::QueryEx& queryExec)
	 : m_queryExec(queryExec)
	{
	}

    std::string query_generator::getSQL() const
    {
		std::vector<std::tuple<std::string /*name*/,std::string/*aggregation*/, common::eDataType>> resultObjects;
		std::ostringstream where;
        std::vector<std::string> group_by;
		std::vector<std::pair<std::string /*name*/,std::string/*order*/>> order_by;

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
			for (const auto& dimension : m_queryExec.getQueryDefinition().getDimensions())
			{
				if( ! ina::query_model::QueryEx::isDimensionOfMeasures(dimension))
				{
					for(const auto& attribut : dimension.getAttributes())
						group_by.push_back(attribut.getName());
				}
			}
			const auto& selectionOperator = m_queryExec.getQueryDefinition().getSelection().getOperator();

			buildWhereClause(selectionOperator, where);

			if (!m_queryExec.getQueryDefinition().getSorts().empty())
			{
				for(const auto& querySort : m_queryExec.getQueryDefinition().getSorts())
				{
					// case of MemberSort, TODO: in the Grid
					if (ina::query_model::QueryEx::DIMENSION_OF_MEASURES_NAME != querySort.getObjectName())
					{
						std::string sortSQL;
						if (querySort.getDirection() == ina::query_model::Sort::Direction::Ascending)
							sortSQL = "ASC";
						else if (querySort.getDirection() == ina::query_model::Sort::Direction::Descending)
							sortSQL = "DESC";

						const std::string& nameSortedDim = querySort.getObjectName();
						for(const auto& dim : m_queryExec.getQueryDefinition().getDimensions())
						{
							if(dim.getName() == nameSortedDim)
							{
								if(m_queryExec.getDSCube() != nullptr)
								{
									if(querySort.getSortType()==ina::query_model::Sort::SortType::MemberKey)
										order_by.push_back(std::make_pair(m_queryExec.getDSCube()->getDimension(dim.getName()).getKeyAttribute().getName(), sortSQL));
									else if(querySort.getSortType()==ina::query_model::Sort::SortType::MemberText)
										order_by.push_back(std::make_pair(m_queryExec.getDSCube()->getDimension(dim.getName()).getTextAttribute().getName(), sortSQL));
									else
										order_by.push_back(std::make_pair(dim.getAttributes().at(0).getName(), sortSQL));
								}
								else // For unit test
								{
									order_by.push_back(std::make_pair(dim.getAttributes().at(0).getName(), sortSQL));
								}
							}
						}
					}
					else
					{
						std::cerr << "WASABI: Sort on member NYI" << std::endl;
					}
				}
			}

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
					if(s.second.empty() == false)
						sql  << " " << s.second;
					if(delim.empty()) delim = ", ";
				}
			}

			sql << ";";
		}
        return sql.str();
    }

	void query_generator::buildWhereSetOperandClause(const ina::query_model::SelectionElement& selectionElement, ina::query_model::LogicalOperator parentLogicalOperator, std::ostringstream& where) const
	{
		std::string SELECTIONS;
		bool first = true;
		for(const auto& element : selectionElement.getElements())
		{
			if ("[Measures].[Measures]" != element.getFieldName())
			{
				if(first == false)
					SELECTIONS += " OR ";
				SELECTIONS += generateSQL(element) ;
				first = false;
			}
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
			 //TODO: Or by default ?
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
