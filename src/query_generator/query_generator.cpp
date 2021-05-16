#include "query_generator.h"

#include <algorithm>
#include <calculator/storage.h>
#include <InA_query_model/Query.h>
#include <InA_query_model/Dimension.h>

#include <sstream>
#include <iostream>
#include <stdexcept>

#include "ModelSQLGenerator.h"

namespace query_generator
{
    std::string query_generator::getSQL(const calculator::DataStorage& data) const
    {
		std::vector<std::string> selected;
		std::ostringstream where;
        std::vector<std::string> group_by;
		std::vector<std::string> order_by;

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
				selected.push_back(dimension.getName());
				
				// Integrity check beetwen query and data storage columns
				{
					if(data.getColIndex(dimension.getName()) != idxInData)
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
					group_by.push_back(dimension.getName());
				}
			}

			for(const auto& filter : m_query.getDefinition().getQueryFilters())
			{
				if (!("[Measures].[Measures]" == filter.getFieldName()))
				{
					if (where.str().empty())
					{
						where << " WHERE ";
					}
					else 
					{
						//TODO: Change when we handle the whole filter tree
						where << " OR ";
					}
					
					where << generateSQL(filter);
				}
			}

			if (!m_query.getDefinition().getQuerySorts().empty())
			{
				for(const auto& querySort : m_query.getDefinition().getQuerySorts())
				{
					// case of MemberSort, TODO: in the Grid
					if (ina::query_model::Dimension::DIMENSION_OF_MEASURES_NAME != querySort.getObjectName())
					{
						order_by.push_back(generateSQL(querySort));
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
					sql << delim << s;
					if(delim.empty()) delim = ", ";
				}
			}

			sql << ";";
		}
        return sql.str();
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
					data.addColumn(dimension.getName(),calculator::eDataType::String, calculator::eColumnType::Indexed);
					break;
				}
				case ina::query_model::Dimension::eAxe::Columns:
				{
					data.addColumn(dimension.getName(),calculator::eDataType::String, calculator::eColumnType::Indexed);
					break;
				}
				default:
					throw std::runtime_error("Unknow axe");
				}
			}
        }
	}
} //query_generator
