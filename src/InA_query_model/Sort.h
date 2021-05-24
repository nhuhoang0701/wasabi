#pragma once
#include "exceptions/InA_Exception.h"

#include <json/jsonReader.h>

#include <string>

namespace ina::query_model
{
    class Sort;
    void read(Sort & querySort, const JSONGenericObject& jsonSort);

    class Sort {
        public:
            enum class SortType {
                Undef,
                MemberKey,
                MemberText,
                Field,
                Datacell,
                Hierarchy,
                Measure,
                Complex
            };

            inline static Sort::SortType getSortType(const std::string& str)
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

            enum class Direction {
                Default,
                Ascending,
                Descending,
                None
            };

            inline static Sort::Direction getDirection(const std::string& str)
            {
                if (str == "Asc")     return Direction::Ascending;
                if (str == "Desc")    return Direction::Descending;
                if (str == "None")          return Direction::None;
                // MDS_TheDefinitveGuide_2_1.pdf
                // P87
                // If not mentioned, the server decides, how to sort.
                return Direction::Default;
            }

            Sort();
            const std::string&     getObjectName() const;
            Direction       getDirection() const;
            SortType        getSortType() const;

            void setObjectName(std::string objectName);
            void setDirection(Direction direction);
            void setSortType(SortType sortType);

            private:
                std::string     _objectName;
                Direction       _direction = Direction::Default;
                SortType        _sortType = SortType::Undef;

    };
}