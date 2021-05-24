#include "Sort.h"

namespace ina::query_model
{
            Sort::Sort() 
            {

            }
            Sort::Direction Sort::getDirection() const
            {
                return _direction;
            }
            const std::string& Sort::getObjectName() const
            {
                return _objectName;
            }
            Sort::SortType Sort::getSortType() const
            {
                return _sortType;
            }

            void Sort::setDirection(Sort::Direction direction)
            {
                _direction = direction;
            }
            void Sort::setObjectName(std::string fieldName)
            {
                _objectName = fieldName;
            }
            void Sort::setSortType(Sort::SortType sortType)
            {
                _sortType = sortType;
            }
}