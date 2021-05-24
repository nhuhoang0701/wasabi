#pragma once

#include <InA_metadata/Cube.h>

#include "Definition.h"

#include <calculator/common.h>   // For calculator::Value

#include <string>

namespace ina::query_model
{
    class Formula;
    class Function;
    class Parameter;
    
	calculator::Value eval(const void* context, const ina::query_model::Formula& fct, void (*getValueCallback)(const void* context, const std::string& nameObj, calculator::Value& value));
	size_t getDeps(const ina::query_model::Formula& fct, std::vector<std::string>& deps);

    size_t getDeps(const ina::query_model::Selection& selection, std::vector<std::string>& deps);

    class QueryEx
    {
    public:
        const static std::string DIMENSION_OF_MEASURES_NAME;

        inline static bool isDimensionOfMeasures(const Dimension & dimension)
        {
            return dimension.getName() == DIMENSION_OF_MEASURES_NAME;
        }

        QueryEx(const Definition& queryDefinition); // For test unit
        QueryEx(const Definition& queryDefinition, const ina::metadata::Cube& dsCube);

        const ina::query_model::Definition&  getQueryDefinition() const;
        const ina::metadata::Cube*           getDSCube() const;

        bool isDataSourceObject(const std::string& attributeName) const;
		const std::vector<Member>    getVisibleMembers(const Dimension& dimension) const;

        private:
        const Definition& m_queryDefinition;
        const ina::metadata::Cube* m_dsCube = nullptr;
    };
}