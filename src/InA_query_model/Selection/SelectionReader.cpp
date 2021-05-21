#include "InA_query_model/Selection/SelectionElement.h"
#include "Selection.h"
#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{
	void read(Selection& selection, const JSONGenericObject& selectionNode)
    {       
        SelectionElement selectionElement;
        JSONGenericObject selectionElementNode;
        if(selectionNode.getObject("Operator"))
        {
            selectionElementNode = selectionNode.getObject("Operator");
            selectionElement = SelectionElement(SelectionElement::Type::Operator);
        } 
        else if (selectionNode.getObject("SetOperand"))
        {
            selectionElementNode = selectionNode.getObject("SetOperand");
            selectionElement = SelectionElement(SelectionElement::Type::SetOperand);
        }
        read(selectionElement, selectionElementNode);
        selection.setOperator(selectionElement);
    }
}
