#include "query_generator.h"

#include <calculator/storage.h>
#include <InA_metadata/Cube.h>
#include <InA_query_model/Query.h>
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
	query_generator::query_generator(const ina::query_model::Query& query)
	 : m_query(query)
	{
	}

    std::string query_generator::getSQL(const calculator::DataStorage& data, const ina::metadata::Cube* dsCube) const
    {
		std::vector<std::pair<std::string /*name*/,std::string/*aggregation*/>> selected;
		std::ostringstream where;
        std::vector<std::string> group_by;
		std::vector<std::pair<std::string /*name*/,std::string/*order*/>> order_by;

		size_t idxInData = 0;
        for (const auto& dimension : m_query.getDefinition().getDimensions())
        {
			if(ina::query_model::Dimension::isDimensionOfMeasures(dimension))
			{
				// Add measures
				for(const auto& member : m_query.getDefinition().getVisibleMembers(dimension) )
				{
					const std::string& memberName = ina::query_model::Member::getName(member);
					if(dsCube == nullptr)// TODO: for unit test
					{
						if(member.getFormula() != nullptr || member.getSelection() != nullptr)
							continue;
					}
					else
					{
						if(dsCube->containsDataSourceColumn(memberName)==false)
							continue;
					}

					// Integrity check beetwen query and data storage columns
					{
						if(data.getColIndex(memberName) != idxInData)
							throw std::runtime_error("Missmatch col. index in data with query");
					}

					std::string agg = member.getAggregation();
					if(agg.empty())
					{
						std::string msg;
						msg += "WASABI: ERROR: Missing aggreation in the InA request for member '" + memberName + "' hardcoded SUM will be used";
						std::cerr << msg;
						agg = "SUM";
					}
					selected.push_back(std::make_pair(memberName, agg));
					idxInData++;
				}
				// Add function dependencies
				for(const auto& member : m_query.getDefinition().getVisibleMembers(dimension) )
				{
					if(member.getFormula() == nullptr)
						continue;

					std::vector<std::string> deps;
					ina::query_model::getDeps(*member.getFormula(), deps);

					for(const auto&memberName : deps )
					{
						// TODO: for unit test
						if(dsCube != nullptr)
						{
							if(dsCube->containsDataSourceColumn(memberName)==false)
								continue;
						}
						bool conatainsIt = false;
						for(const auto& select : selected)
						{
							if(select.first==memberName)
							{
								conatainsIt=true;
								continue;
							}
						}
						if(conatainsIt)
							continue;

						// Integrity check beetwen query and data storage columns
						{
							if(data.getColIndex(memberName) != idxInData)
								throw std::runtime_error("Missmatch col. index in data with query");
						}

						std::string agg = member.getAggregation();
						if(agg.empty())
						{
							std::string msg;
							msg += "WASABI: ERROR: Missing aggreation in the InA request for member '" + memberName + "' hardcoded SUM will be used";
							std::cerr << msg;
							agg = "SUM";
						}
						selected.push_back(std::make_pair(memberName, agg));
						idxInData++;
					}
				}
			}
			else
			{
				for(const auto& attribut : dimension.getAttributes())
					selected.push_back(std::make_pair(attribut.getName(), ""));
				
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

						const std::string& name = querySort.getObjectName();
						for(const auto& dim : m_query.getDefinition().getDimensions())
						{
							if(dim.getName() == name)
							{
								if(dsCube != nullptr)
								{
									if(querySort.getSortType()==ina::query_model::QuerySort::SortType::MemberKey)
										order_by.push_back(std::make_pair(dsCube->getDimension(dim.getName()).getKeyAttribute().getName(), sortSQL));
									else if(querySort.getSortType()==ina::query_model::QuerySort::SortType::MemberText)
										order_by.push_back(std::make_pair(dsCube->getDimension(dim.getName()).getTextAttribute().getName(), sortSQL));
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
			for(const auto& s : selected)
			{
				if(s.second.empty())
	        		sql << delim << s.first;
				else
					sql << delim << s.second << "(" << s.first << ")";
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

	void query_generator::buildWhereSetOperandClause(const ina::query_model::SelectionElement& selectionElement, ina::query_model::LogicalOperator parentLogicalOperator, std::ostringstream& where) const
	{
		for(const auto& element : selectionElement.getElements())
		{
			if ("[Measures].[Measures]" != element.getFieldName())
			{
				std::string code = ina::query_model::toString(parentLogicalOperator);
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
	
	void query_generator::prepareStorage(calculator::DataStorage& data, const ina::metadata::Cube* dsCube) const
	{
		data.clear();
        for (const auto& dimension : m_query.getDefinition().getDimensions())
        {
			if(ina::query_model::Dimension::isDimensionOfMeasures(dimension))
			{
				for(const auto& member : m_query.getDefinition().getVisibleMembers(dimension) )
				{
					const std::string& memberName = ina::query_model::Member::getName(member);
					if(dsCube == nullptr)// TODO: for unit test
					{
						if(member.getFormula() != nullptr || member.getSelection() != nullptr)
							continue;
					}
					else
					{
						if(dsCube->containsDataSourceColumn(memberName)==false)
							continue;
					}

					data.addColumn(memberName,calculator::eDataType::Number, calculator::eColumnType::NoneIndexed);
				}
				for(const auto& member : m_query.getDefinition().getVisibleMembers(dimension) )
				{
					// TODO: Query generator should not use ina::query_model::Dimension,
					// but ina::metadata::Dimension to know if object is part of the data source
					if(member.getFormula() == nullptr )
						continue;
						
					std::vector<std::string> deps;
					ina::query_model::getDeps(*member.getFormula(), deps);

					for(const auto& memberName : deps)
					{
						// TODO: for unit test
						if(dsCube != nullptr)
						{
							if(dsCube->containsDataSourceColumn(memberName)==false)
								continue;
						}
						if(data.haveCol(memberName))
							continue;
						
						data.addColumn(memberName,calculator::eDataType::Number, calculator::eColumnType::NoneIndexed);
					}
				}
			}
			else
			{
				switch (dimension.getAxe())
				{
				case ina::query_model::Dimension::eAxe::Rows:
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
