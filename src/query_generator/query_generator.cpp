#include "query_generator.h"

#include <calculator/storage.h>
#include <InA_query_model/Query.h>
#include <InA_query_model/Dimension.h>

#include <sstream>
#include <iostream>
#include <stdexcept>

#include "ModelSQLGenerator.h"

namespace query_generator
{
    std::string query_generator::getSQL() const
    {
        std::string delim;
		std::ostringstream selected;
		std::ostringstream where;
        std::ostringstream group_by;
		std::ostringstream order_by;

		delim.clear();
        for (const auto& dimension : m_query.getDefinition().getDimensions())
        {
			if(ina::query_model::Dimension::isDimensionOfMeasures(dimension))
			{
				for(const auto& member : m_query.getDefinition().getVisibleMembers(dimension) )
				{
					selected << delim;
					if(delim.empty())
						delim = ", ";
					selected << member.getAggregation() << "(" << member.getName() << ")";
				}
			}
			else
			{
				const std::string& dimensionName = dimension.getName();
				selected << delim;
				selected << dimensionName;
				if(delim.empty())
					delim = ", ";
			}
        }
		delim.clear();
        for (const auto& dimension : m_query.getDefinition().getDimensions())
        {
			if( ! ina::query_model::Dimension::isDimensionOfMeasures(dimension))
			{
				group_by << delim;
				if(delim.empty())
					delim = ", ";
				const std::string& dimensionName = dimension.getName();
				group_by << dimensionName;
			}
        }

		for(const auto & filter : m_query.getDefinition().getQueryFilters())
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
			for(const auto & querySort : m_query.getDefinition().getQuerySorts())
			{
				if (!order_by.str().empty())
				{
					order_by << ", ";
				}

				// case of MemberSort, TODO: in the Grid
				if (ina::query_model::Dimension::DIMENSION_OF_MEASURES_NAME != querySort.getObjectName())
				{
					order_by << generateSQL(querySort);
				}
				else
				{
					std::cerr << "WASABI: Sort on member NYI" << std::endl;
				}
			}
		}

        std::ostringstream sql;
		if(!selected.str().empty())
		{
        	sql << "SELECT ";
        	sql << selected.str();
			const std::string& table = m_query.getDataSource().getObjectName();
			sql << " FROM " << table;
			if (!where.str().empty())
			{
				sql << where.str();
			}
			if(!group_by.str().empty())
			{
				sql << " GROUP BY " << group_by.str();
			}
			if(!order_by.str().empty())
			{
				sql << " ORDER BY " << order_by.str();
			}

			sql << ";";
		}
        return sql.str();
    }

	void query_generator::prepareStorage(calculator::DataStorage& data) const
	{
        for (const auto& dim : m_query.getDefinition().getDimensions())
        {
			if(!ina::query_model::Dimension::isDimensionOfMeasures(dim))
			{
				switch (dim.getAxe())
				{
				case ina::query_model::Dimension::eAxe::Rows:
				{
					data.addColumn(dim.getName(),calculator::eDataType::String, calculator::eColumnType::Indexed);
					break;
				}
				case ina::query_model::Dimension::eAxe::Columns:
				{
					data.addColumn(dim.getName(),calculator::eDataType::String, calculator::eColumnType::Indexed);
					break;
				}
				default:
					throw std::runtime_error("Unknow axe");
				}
			}
        }
		
        for (const auto& dim : m_query.getDefinition().getDimensions())
        {
			if(ina::query_model::Dimension::isDimensionOfMeasures(dim))
			{
				for(const auto& member : dim.getMembers() )
					data.addColumn(member.getName(),calculator::eDataType::Number, calculator::eColumnType::NoneIndexed);
			}
		}
	}
} //query_generator
