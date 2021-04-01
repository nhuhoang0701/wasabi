#include "QuerySort.h"
#include <json/jsonReader.h>
namespace ina::query_model
{
	void read(QuerySort & querySort, const JSONGenericObject& jsonQuerySort)
    {
        std::string direction = jsonQuerySort.getString("Direction");        
        querySort.setDirection(QuerySort::getDirection(direction));

        std::string sortType = jsonQuerySort.getString("SortType");
        querySort.setSortType(QuerySort::getSortType(sortType));    

        switch (querySort.getSortType()) 
        {
            case QuerySort::SortType::MemberKey:
            case QuerySort::SortType::MemberText:
            case QuerySort::SortType::Hierarchy:
            {
                querySort.setObjectName(jsonQuerySort.getString("Dimension"));
                break;
            }

            case QuerySort::SortType::Complex:
            case QuerySort::SortType::Datacell:
            {
                // TODO Not Implemented 
                break;
            }
            case QuerySort::SortType::Field:
            {
                querySort.setObjectName(jsonQuerySort.getString("FieldName"));
                break;
            }

            case QuerySort::SortType::Measure:
            {
                querySort.setObjectName(jsonQuerySort.getString("MeasureName"));
                break;
            }        
        }
    }
}
