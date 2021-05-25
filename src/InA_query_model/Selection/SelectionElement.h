#pragma once
#include "Element.h"
namespace ina::query_model 
{
    enum class LogicalOperator
    {
        And,
        Not,
        Or,
        Undefined
    };

    LogicalOperator getLogicalOperator(const std::string& str);

    std::string toString(const LogicalOperator logicalOperator);

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
        void setCode(LogicalOperator code);
        LogicalOperator getCode() const;
        const std::vector<SelectionElement> & getSubSelections() const;

        private:
        Type _type = Type::Undefined;
        
        // Operator
        LogicalOperator _code = LogicalOperator::Undefined;
        std::vector<SelectionElement> _subSelections;

        // SetOperand
        std::vector<Element> _elements;
        friend void read(SelectionElement& selectionElement, const JSONGenericObject& setOperandNode); 

    };
	class SetOperand
    {
        public:
        SetOperand() = default;
        virtual ~SetOperand() = default;

        const std::vector<Element>& getElements() const;

        private:
        friend void read(SetOperand& selectionElement, const JSONGenericObject& setOperandNode);

        std::string _fieldName;
        std::vector<Element> _elements;
    };
	class Operator
    {
        public:
        Operator() = default;
        ~Operator() = default;

        void setCode(LogicalOperator code);
        LogicalOperator getCode() const;
        const std::vector<SelectionElement> & getSubSelections() const;

        private:
        friend void read(Operator& selectionElement, const JSONGenericObject& setOperandNode);
    
        LogicalOperator _code = LogicalOperator::Undefined;
        std::vector<SetOperand> _subSelections;
    };
}