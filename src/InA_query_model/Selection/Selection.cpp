#include "Selection.h"
#include "SelectionElement.h"

namespace ina::query_model
{
    void Selection::setOperator(const SelectionElement& selectionOperator) 
    {
        _operator = selectionOperator;
    }

    const SelectionElement& Selection::getOperator() const
    {
        return _operator;
    }
}