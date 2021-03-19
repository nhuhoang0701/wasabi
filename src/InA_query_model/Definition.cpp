#include "Definition.h"

#include <iostream>

namespace ina::query_model
{
    void Definition::addDimension(const Dimension& dimension)
    {
        m_objs.push_back(dimension);
    }

    void Definition::addQueryFilter(const InA_queryFilterComparison& queryFilter)
    {
        m_filters.push_back(queryFilter);
    }

	const std::vector<Dimension>& Definition::getDimensions() const
	{
		return m_objs;
	}
	
	const std::vector<InA_queryFilterComparison>& Definition::getQueryFilters() const
	{
		return m_filters;
	}

    const std::vector<Member> Definition::getVisibleMembers(const Dimension& dimension) const 
    {
        if (!m_filters.empty())
        {
            std::vector<Member> visibleMembers;
            for(const auto & member : dimension.getMembers())
            {
                for(const auto & filter : m_filters)
                {
                    /* 
                    MDS_TheDefinitveGuide_2_1 page 237
                    5.4.1 FieldName
                        The name of the field, which is used in the comparison. Even though the term “FieldName”
                        was use, only attribute names may be specified in this field.
                        Even some virtual attributes (e.g. hierarchy key or description) can be used.
                    */
                    bool matchFieldName = false;                    
                    for(const auto & attribute : dimension.getAttributes())
                    {
                        if (filter.getFieldName()  == attribute.getName())
                        {
                            matchFieldName = true;
                            break;
                        }
                    }
                    if (matchFieldName)
                    {
                        if (filter.getComparisonOperator() == InA_queryFilter::ComparisonOperator::EqualTo 
                            && filter.getLowValue() == member.getName())
                        {
                            visibleMembers.push_back(member);
                        }
                    }
                }
            }
            return visibleMembers;
        }
        else
        {
            return dimension.getMembers();
        }
    }

} //query_model