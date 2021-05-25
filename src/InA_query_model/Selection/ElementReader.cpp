
#include "InA_query_model/Selection/Element.h"
#include "InA_query_model/Selection/SelectionElement.h"
#include <cstdlib>
#include <iostream>
#include <json/jsonReader.h>    // For JSONGenericObject
#include <stdexcept>
#include <vector>

namespace ina::query_model 
{
    void read(Element& element, JSONGenericObject elementNode)
    {
        if (elementNode.haveValue("Low"))
        {
            common::Value value;
            if(elementNode.isString("Low"))
                value = elementNode.getString("Low");
            else if(elementNode.isDouble("Low"))
                value = elementNode.getDouble("Low");
            else if(elementNode.isInteger("Low"))
                value = static_cast<double>(elementNode.getInteger("Low"));
            else
                throw std::runtime_error("Elements::Low json datatype NYI");
            element.setLowValue(value);
        }
        
        if (elementNode.haveValue("High"))
        {
            common::Value value;
            if(elementNode.isString("High"))
                value = elementNode.getString("High");
            else if(elementNode.isDouble("High"))
                value = elementNode.getDouble("High");
            else if(elementNode.isInteger("High"))
                value = static_cast<double>(elementNode.getInteger("High"));
            else
                throw std::runtime_error("Elements::High json datatype NYI");
            element.setHighValue(value);
        }
        
        element.setComparisonOperator(Element::getComparisonOperator(elementNode.getString("Comparison")));
        if (elementNode.haveValue("IsExcluding"))
            element.setExcluding(elementNode.getBool("IsExcluding"));

    }
    void read(std::vector<Element> & elements, JSONGenericObject setOperandNode)
    {
        if(const auto& elementNodes = setOperandNode.getArray("Elements"))
        {
            elements.resize(elementNodes.size());
            for(size_t j = 0; j < elementNodes.size(); ++j)
            {	
                read(elements[j], elementNodes[j]);
            }
        }
    }
}