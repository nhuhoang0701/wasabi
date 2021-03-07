#include "InA_queryDefinition.h"

#include <iostream>

namespace ina::query_model
{
    void Definition::addDimension(const InA_dimension& dimension)
    {
        m_objs.push_back(dimension);
    }

    void Definition::addQueryFilter(const InA_queryFilterComparison& queryFilter)
    {
        m_filters.push_back(queryFilter);
    }

	const std::vector<InA_dimension>& Definition::getDimensions() const
	{
		return m_objs;
	}
	
	const std::vector<InA_queryFilterComparison>& Definition::getQueryFilters() const
	{
		return m_filters;
	}

    const std::vector<InA_member>& Definition::getVisibleMembers(const InA_dimension& dimension, std::vector<InA_member> & visibleMembers) const 
    {
        std::vector<InA_member>::const_iterator pMemberIterator;
        for(pMemberIterator = dimension.getMembers().begin(); pMemberIterator != dimension.getMembers().end(); ++ pMemberIterator)
        {
            std::vector<InA_queryFilterComparison>::const_iterator pFilterIterator;
            for(pFilterIterator = m_filters.begin(); pFilterIterator != m_filters.end(); ++ pFilterIterator)
            {
                if ((*pFilterIterator).getFieldName() == dimension.getName()
                    && (*pFilterIterator).getComparisonOperator() == InA_queryFilter::ComparisonOperator::EqualTo 
                    && (*pFilterIterator).getLowValue() == (*pMemberIterator).getName())
                {
                    const InA_member & currentMember = *pMemberIterator;
                    visibleMembers.push_back(currentMember);
                }
            }
        }
        return visibleMembers;
    }

} //query_model