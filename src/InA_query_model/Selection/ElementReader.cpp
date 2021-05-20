
#include "InA_query_model/Selection/Element.h"
#include <json/jsonReader.h>    // For JSONGenericObject
#include <vector>
namespace ina::query_model 
{
    void read(std::vector<Element> & queryFilterComparisons, JSONGenericObject setOperandNode)
    {
        const std::string fieldName = setOperandNode.getString("FieldName");
        if(const auto& elementNodes = setOperandNode.getArray("Elements"))
        {
            for(size_t j = 0; j < elementNodes.size(); ++j)
            {	
                query_model::Element queryFilterComparison(fieldName);
                if (elementNodes[j].haveValue("Low"))
                {
                    std::string lowValue = elementNodes[j].getString("Low");
                    queryFilterComparison.setLowValue(lowValue);
                }
                
                if (elementNodes[j].haveValue("High"))
                {
                    std::string highValue = elementNodes[j].getString("High");
                    queryFilterComparison.setHighValue(highValue);
                }

                std::string comparisonValue = elementNodes[j].getString("Comparison");
                query_model::Element::ComparisonOperator comparisonOperator = query_model::Element::getComparisonOperator(comparisonValue);
                queryFilterComparison.setComparisonOperator(comparisonOperator);
                if (elementNodes[j].haveValue("IsExcluding"))
                {
                    bool isExcluding = elementNodes[j].getBool("IsExcluding");
                    queryFilterComparison.setExcluding(isExcluding);
                }
                queryFilterComparisons.push_back(queryFilterComparison);
            }
        }
    }
}