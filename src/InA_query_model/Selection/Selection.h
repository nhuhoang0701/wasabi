#pragma once
#include "SelectionElement.h"
#include <string>
#include <vector>

class JSONGenericObject;    // #include <json/jsonReader.h>

namespace ina::query_model 
{
    class Selection;
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