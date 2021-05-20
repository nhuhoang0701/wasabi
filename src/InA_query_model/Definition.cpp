#include "Definition.h"

#include <iostream>

namespace ina::query_model
{
    void Definition::addDimension(const Dimension& dimension)
    {
        m_dimensions.push_back(dimension);
    }

    void Definition::addQueryFilter(const QueryFilterComparison& queryFilter)
    {
        m_filters.push_back(queryFilter);
    }

    void Definition::addQuerySort(const QuerySort& querySort)
    {
        m_sorts.push_back(querySort);
    }

	const std::vector<Dimension>& Definition::getDimensions() const
	{
		return m_dimensions;
	}
	
	const std::vector<QueryFilterComparison>& Definition::getQueryFilters() const
	{
		return m_filters;
	}

	const std::vector<QuerySort>& Definition::getQuerySorts() const
	{
		return m_sorts;
	}

    const std::vector<Member> Definition::getVisibleMembers(const Dimension& dimension) const 
    {
        if (!m_filters.empty())
        {
            std::vector<Member> visibleMembers;
            for(const auto& member : dimension.getMembers())
            {
                for(const auto& filter : m_filters)
                {
                    /* 
                    MDS_TheDefinitveGuide_2_1 page 237
                    5.4.1 FieldName
                        The name of the field, which is used in the comparison. Even though the term “FieldName”
                        was use, only attribute names may be specified in this field.
                        Even some virtual attributes (e.g. hierarchy key or description) can be used.
                    */
                    bool matchFieldName = false;                    
                    for(const auto& attribute : dimension.getAttributes())
                    {
                        if (filter.getFieldName()  == attribute.getName())
                        {
                            matchFieldName = true;
                            break;
                        }
                    }
                    if (matchFieldName)
                    {
                        if (filter.getComparisonOperator() == QueryFilter::ComparisonOperator::EqualTo )
                        {
                            if(filter.getLowValue() == ina::query_model::Member::getName(member))
                            {
                                visibleMembers.push_back(member);
                            }
                            //TODO:
                            else if(member.getFormula() != nullptr)
                                continue;
                        }
                    }
                }
            }
            return visibleMembers;
        }

        return dimension.getMembers();
    }

    const ResultSetFeatureRequest& Definition::getResultSetFeat() const
    {
        return m_resultSetFeature;
    }
} //query_model
