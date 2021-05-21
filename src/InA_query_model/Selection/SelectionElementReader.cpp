#include "InA_query_model/Selection/SelectionElement.h"
#include "SelectionElement.h"
#include "Element.h"
#include <iostream>
#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{


	void read(SelectionElement& selectionElement, const JSONGenericObject& selectionElementNode)
    {
        if (selectionElement.getType() == SelectionElement::Type::Operator)
        {
            std::cout << "Reading Operator" << std::endl;
            std::string code = selectionElementNode.getString("Code");
            Element::LogicalOperator logicalOperator = Element::getLogicalOperator(code);
            selectionElement.setCode(logicalOperator);
            if (const auto & subSelections = selectionElementNode.getArray("SubSelections"))
            {
                std::cout << "Node:SubSelections" << std::endl;
                for(size_t i = 0; i < subSelections.size(); ++i)
                {
                    const auto& subSelection = subSelections[i];
                    
                    if (const auto& childOperatorNode = subSelection.getObject("Operator"))
                    {
                        std::cout << "Node:Operator" << std::endl;
                        SelectionElement childSelectionElement(SelectionElement::Type::Operator);
                        read(childSelectionElement, childOperatorNode);
                        selectionElement._subSelections.push_back(childSelectionElement);
                        std::cout << "childSelectionElement._subSelection == " << childSelectionElement._subSelections.size() << std::endl;
                    }
                    else if (const auto& setOperandNode = subSelection.getObject("SetOperand"))
                    {
                        std::cout << "Node:SetOperand" << std::endl;
                        SelectionElement childSelectionElement(SelectionElement::Type::SetOperand);
                        read(childSelectionElement._elements, setOperandNode);
                        std::cout << "childSelectionElement._elements == " << childSelectionElement._elements.size() << std::endl;
                        selectionElement._subSelections.push_back(childSelectionElement);
                    }
                }
            }
            else 
            {
                std::cout << "Error !!!" << std::endl;
            }
                
            
            
        }
        else if (selectionElement.getType() == SelectionElement::Type::SetOperand)
        {
            std::cout << "Reading Root SetOperand" << std::endl;
            SelectionElement childSelectionElement(SelectionElement::Type::SetOperand);
            read(childSelectionElement._elements, selectionElementNode);
            selectionElement._subSelections.push_back(childSelectionElement);
        } 
    }
}
