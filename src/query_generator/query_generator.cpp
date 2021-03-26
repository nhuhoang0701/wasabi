#include "query_generator.h"

#include <calculator/storage.h>
#include <InA_query_model/Query.h>
#include <InA_query_model/Dimension.h>

#include <sstream>
#include <iostream>
#include <stdexcept>

namespace query_generator
{
    std::string query_generator::getSQL() const
    {
        std::string delim;
        std::ostringstream sql;
		std::ostringstream where;
        std::ostringstream group_by;
        
        sql << "SELECT ";

        for (const auto& dimension : m_query.getDefinition().getDimensions())
        {
			if(ina::query_model::Dimension::isDimensionOfMeasures(dimension))
			{
				for(const auto& member : m_query.getDefinition().getVisibleMembers(dimension) )
				{
					sql << delim;
					sql << member.getAggregation() << "(" << member.getName() << ")";
				}
			}
			else
			{
				const std::string& dimensionName = dimension.getName();
				sql << delim;
				sql << dimensionName;
				group_by << delim << dimensionName;
			}

            delim = ", ";
        }

        const std::string& table = m_query.getDataSource().getObjectName();
        sql << " FROM " << table;

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
					//TODO: Change when we handle logical operators
					where << " AND ";
				}
				
				if (filter.isExcluding())
				{
					where << "NOT ( ";
				}
				// fieldName is never empty 
				where << filter.getFieldName() << " " ;
				// comparisonOperator is never empty 
				where << ina::query_model::InA_queryFilter::comparisonOperatorToString(filter.getComparisonOperator());
				// In case of unary operator, now low/high value
				if (!filter.getLowValue().empty())
				{
					where << " " << filter.getLowValue();
				}
				if (filter.isExcluding())
				{
					where << " )";
				}
			}
		}
		if (!where.str().empty())
		{
			sql << where.str();
		}
        if(!group_by.str().empty())
        {
            sql << " GROUP BY " << group_by.str();
        }

        sql << ";";

        return sql.str();
    }

	void query_generator::prepareStorage(calculator::DataStorage& data) const
	{
        for (const auto& dim : m_query.getDefinition().getDimensions())
        {
			const std::string& nameDim = dim.getName();
			if(nameDim == "CustomDimension1")
			{
				for(const auto& member : dim.getMembers() )
					data.addColumn(member.getName(),calculator::eDataType::Number, calculator::eColumnType::NoneIndexed);
			}
			else
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
	}
} //query_generator
