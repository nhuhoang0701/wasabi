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
		std::vector<std::pair<std::string /*name*/,std::string/*aggregation*/>> selected;
		std::ostringstream where;
        std::vector<std::string> group_by;
		std::vector<std::pair<std::string /*name*/,std::string/*order*/>> order_by;

		// SELECT objects
		{
			for (const auto& dimension : m_queryExec.getQueryDefinition().getDimensions())
			{
				// Add members
				for(const auto& member : m_queryExec.getVisibleMembers(dimension) )
				{
					const std::string& memberName = ina::query_model::Member::getName(member);
					if(m_queryExec.isDataSourceObject(memberName)==false)
						continue;

					std::string agg = member.getAggregation();
					if(agg.empty())
					{
						std::string msg;
						msg += "WASABI: ERROR: Missing aggreation in the InA request for member '" + memberName + "' hardcoded SUM will be used, NYI metadata from SQL tables";
						std::cerr << msg;
						agg = "SUM";
					}
					selected.push_back(std::make_pair(memberName, agg));
				}
				// From formula
				for(const auto& member : m_queryExec.getVisibleMembers(dimension) )
				{
					if(member.getFormula() == nullptr)
						continue;

					std::vector<std::string> deps;
					ina::query_model::getDeps(*member.getFormula(), deps);

					for(const auto&memberName : deps )
					{
						if(m_queryExec.isDataSourceObject(memberName)==false)
							continue;

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

						std::string agg = member.getAggregation();
						if(agg.empty())
						{
							std::string msg;
							msg += "WASABI: ERROR: Missing aggreation in the InA request for member '" + memberName + "' hardcoded SUM will be used";
							std::cerr << msg;
							agg = "SUM";
						}
						selected.push_back(std::make_pair(memberName, agg));
					}
				}
				//From attributes
				for(const auto& attribut : dimension.getAttributes())
				{
					if(m_queryExec.isDataSourceObject(attribut.getName())==false)
						continue;
					selected.push_back(std::make_pair(attribut.getName(), ""));
				}
			}

			// Integrity check index beetwen query and data storage columns
			if(m_dataStorage)
			{
				size_t idxInData = 0;
				for(const auto& select : selected)
				{
					if(m_dataStorage->getColIndex(select.first) != idxInData)
						throw std::runtime_error("Missmatch col. index in data with query");
					idxInData++;
				}
			}
		}

        std::ostringstream sql;
		if(!selected.empty())
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
			for(const auto& s : selected)
			{
				if(s.second.empty())
	        		sql << delim << s.first;
				else
					sql << delim << s.second << "(" << s.first << ")";
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
	
	void query_generator::prepareStorage(std::shared_ptr<calculator::DataStorage> data) const
	{
		if(data.get() == nullptr)
			throw std::runtime_error("query_generator::prepareStorage datastorage should not be null");

        for (const auto& dimension : m_queryExec.getQueryDefinition().getDimensions())
        {
			for(const auto& member : m_queryExec.getVisibleMembers(dimension) )
			{
				const std::string& memberName = ina::query_model::Member::getName(member);

				if(m_queryExec.isDataSourceObject(memberName)==false)
					continue;
				data->addColumn(memberName,calculator::eDataType::Number, calculator::eColumnType::NoneIndexed);
			}
			for(const auto& member : m_queryExec.getVisibleMembers(dimension) )
			{
				// TODO: Query generator should not use ina::query_model::Dimension,
				// but ina::metadata::Dimension to know if object is part of the data source
				if(member.getFormula() == nullptr )
					continue;
					
				std::vector<std::string> deps;
				ina::query_model::getDeps(*member.getFormula(), deps);

				for(const auto& memberName : deps)
				{
					if(m_queryExec.isDataSourceObject(memberName)==false)
						continue;
					if(data->haveCol(memberName))
						continue;
					data->addColumn(memberName,calculator::eDataType::Number, calculator::eColumnType::NoneIndexed);
				}
			}
			//From attribut
			for(const auto& attribut : dimension.getAttributes())
			{
				if(m_queryExec.isDataSourceObject(attribut.getName())==false)
					continue;
				data->addColumn(attribut.getName(),calculator::eDataType::String, calculator::eColumnType::Indexed);
			}
        }
		m_dataStorage = data;
	}
} //query_generator
