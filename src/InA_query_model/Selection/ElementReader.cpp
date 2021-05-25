
#include "InA_query_model/Selection/Element.h"
#include <cstdlib>
#include <iostream>
#include <json/jsonReader.h>    // For JSONGenericObject
#include <stdexcept>
#include <vector>
namespace ina::query_model 
{
    void read(std::vector<Element> & elements, JSONGenericObject setOperandNode)
    {
        const std::string fieldName = setOperandNode.getString("FieldName");
        if(const auto& elementNodes = setOperandNode.getArray("Elements"))
        {
            for(size_t j = 0; j < elementNodes.size(); ++j)
            {	
                query_model::Element element(fieldName);
                if (elementNodes[j].haveValue("Low"))
                {
                    common::Value value;
                    if(elementNodes[j].isString("Low"))
                        value = elementNodes[j].getString("Low");
                    else if(elementNodes[j].isDouble("Low"))
                        value = elementNodes[j].getDouble("Low");
                    else if(elementNodes[j].isInteger("Low"))
                        value = static_cast<double>(elementNodes[j].getInteger("Low"));
                    else
                        throw std::runtime_error("Elements::Low json datatype NYI");
                    element.setLowValue(value);
                }
                
                if (elementNodes[j].haveValue("High"))
                {
                    common::Value value;
                    if(elementNodes[j].isString("High"))
                        value = elementNodes[j].getString("High");
                    else if(elementNodes[j].isDouble("High"))
                        value = elementNodes[j].getDouble("High");
                    else if(elementNodes[j].isInteger("High"))
                        value = static_cast<double>(elementNodes[j].getInteger("High"));
                    else
                        throw std::runtime_error("Elements::High json datatype NYI");
                    element.setHighValue(value);
                }

                std::string comparisonValue = elementNodes[j].getString("Comparison");
                query_model::Element::ComparisonOperator comparisonOperator = query_model::Element::getComparisonOperator(comparisonValue);
                element.setComparisonOperator(comparisonOperator);
                if (elementNodes[j].haveValue("IsExcluding"))
                {
                    element.setExcluding(elementNodes[j].getBool("IsExcluding"));
                }
                elements.push_back(element);
            }
        }
    }
}