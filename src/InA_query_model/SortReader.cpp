#include "Sort.h"

#include <json/jsonReader.h>

namespace ina::query_model
{
	void read(Sort & querySort, const JSONGenericObject& jsonSort)
    {
        if(jsonSort.haveValue("Direction"))
        {
            std::string direction = jsonSort.getString("Direction");
            querySort.setDirection(Sort::getDirection(direction));
        }
        else
            ;//TODO: Error ?

        std::string sortType = jsonSort.getString("SortType");
        querySort.setSortType(Sort::getSortType(sortType));    

        switch (querySort.getSortType()) 
        {
            case Sort::SortType::MemberKey:
            case Sort::SortType::MemberText:
            case Sort::SortType::Hierarchy:
            {
                querySort.setObjectName(jsonSort.getString("Dimension"));
                break;
            }

            case Sort::SortType::Complex:
            case Sort::SortType::Datacell:
            {
                // TODO Not Implemented 
                break;
            }
            case Sort::SortType::Field:
            {
                querySort.setObjectName(jsonSort.getString("FieldName"));
                break;
            }

            case Sort::SortType::Measure:
            {
                querySort.setObjectName(jsonSort.getString("MeasureName"));
                break;
            } 
            default:
            ;//TODO:: Error     
        }
    }
}
