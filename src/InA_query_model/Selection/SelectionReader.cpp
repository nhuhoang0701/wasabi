#include "InA_query_model/Selection/SelectionElement.h"
#include "Selection.h"

#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{
	void read(Selection& selection, const JSONGenericObject& selectionNode)
    {       
        SelectionElement selectionOperator;
        JSONGenericObject operatorNode;
        if(selectionNode.getObject("Operator"))
        {
            operatorNode = selectionNode.getObject("Operator");
            selectionOperator = SelectionElement(SelectionElement::Type::Operator);
        } else if (selectionNode.getObject("SetOperand"))
        {
            operatorNode = selectionNode.getObject("SetOperand");
            selectionOperator = SelectionElement(SelectionElement::Type::SetOperand);
        }
        read(selectionOperator, operatorNode);
        selection.setOperator(selectionOperator);
    }
}
