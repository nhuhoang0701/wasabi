#include "InA_query_model/Selection/SelectionElement.h"
#include "Selection.h"

#include <json/jsonReader.h>    // For JSONGenericObject

namespace ina::query_model
{
	void read(Selection& selection, const JSONGenericObject& selectionNode)
    {       
        auto operatorNode = selectionNode.getObject("Operator");
        SelectionElement selectionOperator(SelectionElement::Type::Operator);
        read(selectionOperator, operatorNode);
        selection.setOperator(selectionOperator);
    }
}
