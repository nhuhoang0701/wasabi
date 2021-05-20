#include "Function.h"

#include <exception>
#include <cmath> // For std::pow
#include <stdexcept>

namespace ina::query_model
{
    double getDouble(const calculator::Value& val)
    {
        if(std::holds_alternative<double>(val))
        {
            return std::get<double>(val);
        }
        if(std::holds_alternative<std::string>(val))
        {
            return std::stod(std::get<std::string>(val));
        }
        throw std::runtime_error("getDouble: Not a double");
    }

    calculator::Value eval(const void* context, const ina::query_model::Function& fct, void (*getValueCallback)(const void* context, const std::string& nameObj, calculator::Value& value))
    {
        std::vector<calculator::Value>  paramValues(fct.getParameterCount());
        for(size_t i = 0; i < fct.getParameterCount(); i++)
        {
            const ina::query_model::Parameter& param = fct.getParameter(i);
            paramValues[i] = eval(context, param, getValueCallback);
        }

        switch(fct.getFunctionType())
        {
            case ina::query_model::Function::eAdd:
            {
                return getDouble(paramValues[0]) + getDouble(paramValues[1]);
            }
            case ina::query_model::Function::eSubtract:
            {
                return getDouble(paramValues[0]) - getDouble(paramValues[1]);
            }
            case ina::query_model::Function::eMultipy:
            {
                return getDouble(paramValues[0]) * getDouble(paramValues[1]);
            }
            case ina::query_model::Function::eDivide:
            {
                return getDouble(paramValues[0]) / getDouble(paramValues[1]);
            }
            case ina::query_model::Function::eSquare:
            {
                return std::pow(getDouble(paramValues[0]), getDouble(paramValues[1]));
            }
            case ina::query_model::Function::eDecFloat:
            {
                return std::stod(std::get<std::string>(paramValues[0]));
            }
            default:
                throw std::runtime_error("Not yet implemented function parameter type");

        }
        return 42.0;
    }

    size_t getDeps(const ina::query_model::Function& fct, std::vector<std::string>& deps)
    {
        size_t nbOfDeps = 0;
        for(size_t i = 0; i < fct.getParameterCount(); i++)
        {
            const ina::query_model::Parameter& param = fct.getParameter(i);
            switch (param.getType())
            {
                case ina::query_model::Parameter::eConstant:
                    continue;
                case ina::query_model::Parameter::eFunction:
                {
                    nbOfDeps += getDeps(param.getFunction(), deps);
                    break;
                }
                case ina::query_model::Parameter::eMember:
                {
                    deps.push_back(param.getName());
                    nbOfDeps++;
                    break;
                }
                default:
                    throw std::runtime_error("Unkonw function parameter type");
            }
        }

        return nbOfDeps;
    }
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
