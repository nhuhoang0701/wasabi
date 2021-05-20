#pragma once
#include "Element.h"
namespace ina::query_model 
{
	class SelectionElement 
    {
        public:
        enum class Type {
            Operator,
            SetOperand,
            Undefined
        };

            SelectionElement() = default;
            SelectionElement(Type type);
            Type getType() const;
            virtual ~SelectionElement() = default;

            // SetOperand
            const std::vector<Element>& getElements() const;

            // Operator
            void setCode(Element::LogicalOperator code);
            Element::LogicalOperator getCode() const;
            const std::vector<SelectionElement> & getSubSelections() const;

        private:
        Type _type = Type::Undefined;
        
        // Operator
        Element::LogicalOperator _code = Element::LogicalOperator::Undefined;
        std::vector<SelectionElement> _subSelections;

        // SetOperand
        std::vector<Element> _elements;
        friend void read(SelectionElement& selectionElement, const JSONGenericObject& setOperandNode); 

    };
}