#include "Definition.h"
#include "InA_query_model/Selection/Element.h"
#include "InA_query_model/Selection/SelectionElement.h"

#include <iostream>
#include <iterator>

namespace ina::query_model
{
    void Definition::addDimension(const Dimension& dimension)
    {
        m_dimensions.push_back(dimension);
    }

    void Definition::addQuerySort(const QuerySort& querySort)
    {
        m_sorts.push_back(querySort);
    }

	const std::vector<Dimension>& Definition::getDimensions() const
	{
		return m_dimensions;
	}
	
	const Selection & Definition::getSelection() const
	{
		return m_selection;
	}

    void Definition::setSelection(Selection & selection)
	{
		m_selection = selection;
	}

	const std::vector<QuerySort>& Definition::getQuerySorts() const
	{
		return m_sorts;
	}

    void Definition::traverse(std::vector<Element>& filters, const SelectionElement& selectionElement) const
    {
        if (selectionElement.getType() == SelectionElement::Type::Operator) 
        {
            if (!selectionElement.getSubSelections().empty())
            {
                for(const auto& childSelectionElement : selectionElement.getSubSelections()) 
                {
                    traverse(filters, childSelectionElement);
                }
            }
        }
        else if (selectionElement.getType() == SelectionElement::Type::SetOperand)
        {
            if (!selectionElement.getElements().empty())
            {
                filters.insert(std::end(filters), std::begin(selectionElement.getElements()), std::end(selectionElement.getElements()));
            }
        }
    }
    const std::vector<Member> Definition::getVisibleMembers(const Dimension& dimension) const 
    {
        std::vector<Element> filters;
        traverse(filters, m_selection.getOperator());
        if (!filters.empty())
        {
            std::vector<Member> visibleMembers;
            for(const auto& member : dimension.getMembers())
            {
                for(const auto& filter : filters)
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
                        if (filter.getComparisonOperator() == Element::ComparisonOperator::EqualTo )
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
        else
        {
            return dimension.getMembers();
        }
    }

    const ResultSetFeatureRequest& Definition::getResultSetFeat() const
    {
        return m_resultSetFeature;
    }
} //query_model
