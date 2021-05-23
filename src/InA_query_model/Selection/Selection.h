#pragma once
#include "SelectionElement.h"
#include <string>
#include <vector>

class JSONGenericObject;    // #include <json/jsonReader.h>

namespace ina::query_model 
{
    class Selection;
    size_t getDeps(const ina::query_model::Selection& selection, std::vector<std::string>& deps);
	class Selection
    {
        public:
        Selection() = default;
        void setOperator(const SelectionElement& selectionOperator);
        const SelectionElement& getOperator() const;

        private:
            SelectionElement _operator;
            friend void read(Selection & obj, const JSONGenericObject& jsonNode);
    };
}