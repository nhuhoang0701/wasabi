#include "InA_query_model/Selection/SelectionElement.h"
#include "SelectionElement.h"
#include "Element.h"

#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{


	void read(SelectionElement& selectionElement, const JSONGenericObject& operatorNode)
    {       
        std::string code = operatorNode.getString("Code");
        Element::LogicalOperator logicalOperator = Element::getLogicalOperator(code);
        selectionElement.setCode(logicalOperator);

        if (const auto & subSelections = operatorNode.getArray("SubSelections"))
        {
            for(size_t i = 0; i < subSelections.size(); ++i)
            {
                const auto& subSelection = subSelections[i];
                
                if (const auto& childOperatorNode = subSelection.getObject("Operator"))
                {
                    SelectionElement childSelectionElement(SelectionElement::Type::Operator);
                    read(childSelectionElement, childOperatorNode);
                    selectionElement._subSelections.push_back(childSelectionElement);
                }
                else if (const auto& setOperandNode = subSelection.getObject("SetOperand"))
                {
                    SelectionElement childSelectionElement(SelectionElement::Type::SetOperand);
                    read(childSelectionElement._elements, setOperandNode);
                    selectionElement._subSelections.push_back(childSelectionElement);
                }
            }
        }
    }
}
