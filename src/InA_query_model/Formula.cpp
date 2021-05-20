#include "Formula.h"

namespace ina::query_model
{
    calculator::Value eval(const void* context, const ina::query_model::Formula& formula, void (*getValueCallback)(const void* context, const std::string& nameObj, calculator::Value& value))
    {
        return eval(context, formula.getParameter(), getValueCallback);
    }
    size_t getDeps(const ina::query_model::Formula& formula, std::vector<std::string>& deps)
    {
        return getDeps(formula.getParameter(), deps);
    }

    const Parameter& Formula::getParameter() const
    {
        return m_parameter;
    }
}
