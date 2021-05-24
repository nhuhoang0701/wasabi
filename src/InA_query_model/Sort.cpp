#include "Sort.h"

namespace ina::query_model
{
            Sort::SortType Sort::getSortType(const std::string& str)
            {
                if (str == "MemberKey")     return SortType::MemberKey;
                if (str == "MemberText")    return SortType::MemberText;
                if (str == "Field")         return SortType::Field;
                if (str == "Datacell")      return SortType::Datacell;
                if (str == "Hierarchy")     return SortType::Hierarchy;
                if (str == "Measure")       return SortType::Measure;
                if (str == "Complex")       return SortType::Complex;
                throw TRACED_InA_EXCEPTION("Sort::SortType");
            }

            Sort::Direction Sort::getDirection(const std::string& str)
            {
                if (str == "Asc")     return Direction::Ascending;
                if (str == "Desc")    return Direction::Descending;
                if (str == "None")          return Direction::None;
                // MDS_TheDefinitveGuide_2_1.pdf
                // P87
                // If not mentioned, the server decides, how to sort.
                return Direction::Default;
            }
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