#include "Function.h"

namespace ina::query_model
{
    Function::Function(const std::string& name, const std::string& desc, eFunctionType type, const std::vector<Parameter>& params):
    m_name(name),
    m_description(desc),
    m_type(type),
    m_vParams(params)
    {

    }

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
