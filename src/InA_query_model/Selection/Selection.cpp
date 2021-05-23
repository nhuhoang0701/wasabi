#include "Selection.h"
#include "SelectionElement.h"
#include <stdexcept>

namespace ina::query_model
{
    size_t getDeps(const ina::query_model::Selection& selection, std::vector<std::string>& deps)
    {
        throw std::runtime_error("NYI Selection dependencies");
    }
    void Selection::setOperator(const SelectionElement& selectionOperator) 
    {
        _operator = selectionOperator;
    }

    const SelectionElement& Selection::getOperator() const
    {
        return _operator;
    }
}