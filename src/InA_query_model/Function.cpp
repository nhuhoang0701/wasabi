#include "Function.h"

namespace ina::query_model
{
    const std::string& Function::getName() const
    {
        return m_name;
    }

    const std::string& Function::getDescription() const
    {
        return m_description;
    }

    Function::eFunctionType Function::getFunctionType() const
    {
        return m_type;
    }

    void Function::addParameter(const Parameter &param)
    {
        m_vParams.push_back(param);
    }

    size_t Function::getParameterCount() const
    {
        return m_vParams.size();
    }

    const Parameter& Function::getParameter(size_t index) const
    {
        return m_vParams[index];
    }

}
