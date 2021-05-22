#include "query_generator.h"

#include <algorithm>
#include <calculator/storage.h>
#include <InA_query_model/Query.h>
#include <InA_query_model/Dimension.h>

#include <ostream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "InA_query_model/Selection/Element.h"
#include "InA_query_model/Selection/SelectionElement.h"
#include "ModelSQLGenerator.h"

namespace query_generator
{
	query_generator::query_generator(const ina::query_model::Query& query)
	 : m_query(query)
	{
	}

    std::string query_generator::getSQL(const calculator::DataStorage& data) const
    {
		std::vector<std::string> selected;
		std::ostringstream where;
        std::vector<std::string> group_by;
		std::vector<std::pair<std::string /*name*/,std::string/*order*/>> order_by;

		size_t idxInData = 0;
        for (const auto& dimension : m_query.getDefinition().getDimensions())
        {
			if(ina::query_model::Dimension::isDimensionOfMeasures(dimension))
			{
				for(const auto& member : m_query.getDefinition().getVisibleMembers(dimension) )
				{
					// TODO: Query generator should not use ina::query_model::Dimension,
					// but ina::metadata::Dimension reduced by the ina::query_model::Definition
					if(member.getFormula() != nullptr)
						continue;

					std::string memberName = ina::query_model::Member::getName(member);
					// Integrity check beetwen query and data storage columns
					{
						if(data.getColIndex(memberName) != idxInData)
							throw std::runtime_error("Missmatch col. index in data with query");
						idxInData++;
					}

					std::string agg = member.getAggregation();
					if(agg.empty())
					{
						std::string msg;
						msg += "WASABI: ERROR: Missing aggreation in the InA request for member '" + memberName + "' hardcoded SUM will be used";
						std::cerr << msg;
						agg = "SUM";
					}
					std::string select = agg + "(" + memberName + ")";
					selected.push_back(select);
					
				}
			}
			else
			{
				for(const auto& attribut : dimension.getAttributes())
					selected.push_back(attribut.getName());
				
				// Integrity check beetwen query and data storage columns
				for(const auto& attribut : dimension.getAttributes())
				{
					if(data.getColIndex(attribut.getName()) != idxInData)
						throw std::runtime_error("Missmatch col. index in data with query");
					idxInData++;
				}
			}
        }

        std::ostringstream sql;
		if(!selected.empty())
		{
			for (const auto& dimension : m_query.getDefinition().getDimensions())
			{
				if( ! ina::query_model::Dimension::isDimensionOfMeasures(dimension))
				{
					for(const auto& attribut : dimension.getAttributes())
						group_by.push_back(attribut.getName());
				}
			}
			const auto& selectionOperator = m_query.getDefinition().getSelection().getOperator();

			buildWhereClause(selectionOperator, where);

			if (!m_query.getDefinition().getQuerySorts().empty())
			{
				for(const auto& querySort : m_query.getDefinition().getQuerySorts())
				{
					// case of MemberSort, TODO: in the Grid
					if (ina::query_model::Dimension::DIMENSION_OF_MEASURES_NAME != querySort.getObjectName())
					{
						std::string sortSQL;
						if (querySort.getDirection() == ina::query_model::QuerySort::Direction::Ascending)
							sortSQL = "ASC";
						else if (querySort.getDirection() == ina::query_model::QuerySort::Direction::Descending)
							sortSQL = "DESC";

						std::string name = querySort.getObjectName();
						for(const auto& dim : m_query.getDefinition().getDimensions())
						{
							if(dim.getName() == name)
							{
								//TODO: Should sort on the good attribut, not all
								for(const auto& attribut : dim.getAttributes())
								{
									order_by.push_back(std::make_pair(attribut.getName(), sortSQL));
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
			for(const auto& s : selected)
			{
        		sql << delim << s;
				if(delim.empty()) delim = ", ";
			}

			const std::string& table = m_query.getDataSource().getObjectName();
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

	void query_generator::buildWhereSetOperandClause(const ina::query_model::SelectionElement& selectionElement, ina::query_model::Element::LogicalOperator parentLogicalOperator, std::ostringstream& where) const
	{
		for(const auto& element : selectionElement.getElements())
		{
			if (!("[Measures].[Measures]" == element.getFieldName()))
			{
				std::string code = ina::query_model::Element::toString(parentLogicalOperator);
				if (where.str().empty())
				{
					where << " WHERE ";
				}
				else
				{							
					where << " " << code << " ";
				}
				std::string generatedSQL = generateSQL(element) ;
				where << generatedSQL;
			}
		}
	}

	void query_generator::buildWhereClause(const ina::query_model::SelectionElement& selectionElement, std::ostringstream& where) const
	{
		if (selectionElement.getType() == ina::query_model::SelectionElement::Type::SetOperand)
		{
			buildWhereSetOperandClause(selectionElement, ina::query_model::Element::LogicalOperator::Undefined, where);
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
	
	void query_generator::prepareStorage(calculator::DataStorage& data) const
	{
		data.clear();
        for (const auto& dimension : m_query.getDefinition().getDimensions())
        {
			if(ina::query_model::Dimension::isDimensionOfMeasures(dimension))
			{
				for(const auto& member : m_query.getDefinition().getVisibleMembers(dimension) )
				{
					// TODO: Query generator should not use ina::query_model::Dimension,
					// but ina::metadata::Dimension reduced by the ina::query_model::Definition
					if(member.getFormula() != nullptr)
						continue;
					std::string memberName = ina::query_model::Member::getName(member);
					data.addColumn(memberName,calculator::eDataType::Number, calculator::eColumnType::NoneIndexed);
				}
			}
			else
			{
				switch (dimension.getAxe())
				{
				case ina::query_model::Dimension::eAxe::Rows:
				{
					for(const auto& attribut : dimension.getAttributes())
						data.addColumn(attribut.getName(),calculator::eDataType::String, calculator::eColumnType::Indexed);
					break;
				}
				case ina::query_model::Dimension::eAxe::Columns:
				{
					for(const auto& attribut : dimension.getAttributes())
						data.addColumn(attribut.getName(),calculator::eDataType::String, calculator::eColumnType::Indexed);
					break;
				}
				default:
					throw std::runtime_error("Unknown axe");
				}
			}
        }
	}
} //query_generator
