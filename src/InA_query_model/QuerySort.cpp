#include "QuerySort.h"

namespace ina::query_model
{
            QuerySort::QuerySort() 
            {

            }
            QuerySort::Direction QuerySort::getDirection() const
            {
                return _direction;
            }
            std::string QuerySort::getObjectName() const
            {
                return _objectName;
            }
            QuerySort::SortType QuerySort::getSortType() const
            {
                return _sortType;
            }

            void QuerySort::setDirection(QuerySort::Direction direction)
            {
                _direction = direction;
            }
            void QuerySort::setObjectName(std::string fieldName)
            {
                _objectName = fieldName;
            }
            void QuerySort::setSortType(QuerySort::SortType sortType)
            {
                _sortType = sortType;
            }
}