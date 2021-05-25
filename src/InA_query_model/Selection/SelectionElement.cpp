#include "SelectionElement.h"
#include <stdexcept>

namespace ina::query_model 
{
    LogicalOperator getLogicalOperator(const std::string& str)
    {
        if (str == "And")
            return LogicalOperator::And;
        else if (str == "Not")
            return LogicalOperator::Not;
        else if (str == "Or")
            return LogicalOperator::Or;
        
        throw std::runtime_error("InA Selection Operator: unknow: '"+str+"'" );
    }
    
    std::string toString(const LogicalOperator logicalOperator) 
    {
        if (logicalOperator == LogicalOperator::And)
            return "AND";
        else if (logicalOperator == LogicalOperator::Not)
            return "NOT";
        else if (logicalOperator == LogicalOperator::Or)
            return "OR";
        else if (logicalOperator == LogicalOperator::Undefined)  
            throw std::runtime_error("InA Selection Operator: 'Undefined' enum value" );;

        throw std::runtime_error("InA Selection Operator: unknow: 'LogicalOperator'" );
    }

	SelectionElement::SelectionElement(Type type) 
    {
        _type = type;
    }

    SelectionElement::Type SelectionElement::getType() const
    {
        return _type;
    }

    void SelectionElement::setCode(LogicalOperator code) 
    {
        _code = code;
    }

    LogicalOperator SelectionElement::getCode() const
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



    void Operator::setCode(LogicalOperator code) 
    {
        _code = code;
    }
}