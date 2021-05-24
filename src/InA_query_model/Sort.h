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
            static Sort::SortType getSortType(const std::string& str);

            enum class Direction {
                Default,
                Ascending,
                Descending,
                None
            };
            static Sort::Direction getDirection(const std::string& str);

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