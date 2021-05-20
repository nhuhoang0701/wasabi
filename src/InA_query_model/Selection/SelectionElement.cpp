#include "SelectionElement.h"
namespace ina::query_model 
{

	SelectionElement::SelectionElement(Type type) 
    {
        _type = type;
    }

    SelectionElement::Type SelectionElement::getType() const
    {
        return _type;
    }

    void SelectionElement::setCode(Element::LogicalOperator code) 
    {
        _code = code;
    }

    Element::LogicalOperator SelectionElement::getCode() const
    {
        return _code;
    }

    const std::vector<SelectionElement> & SelectionElement::getSubSelections() const
    {
        return _subSelections;
    }
    
    const std::vector<Element>& SelectionElement::getElements() const
    {
        return _elements;
    }
}