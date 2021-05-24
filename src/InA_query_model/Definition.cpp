#include "Definition.h"
#include "Selection/Element.h"
#include "Selection/SelectionElement.h"

#include <iostream>
#include <iterator>

namespace ina::query_model
{
    void Definition::addDimension(const Dimension& dimension)
    {
        m_dimensions.push_back(dimension);
    }

    void Definition::addSort(const Sort& querySort)
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

	const std::vector<Sort>& Definition::getSorts() const
	{
		return m_sorts;
	}

    const ResultSetFeatureRequest& Definition::getResultSetFeat() const
    {
        return m_resultSetFeature;
    }
} //query_model
