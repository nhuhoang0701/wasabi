#include "Parameter.h"
#include "Function.h"

#include "calculator/common.h"
#include <cstddef>
#include <exceptions/InA_Exception.h>

namespace ina::query_model
{

	calculator::Value eval(const void* context, const ina::query_model::Parameter& param, void (*getValueCallback)(const void* context, const std::string& nameObj, calculator::Value& value))
    {
        calculator::Value paramValue;
        switch (param.getType())
        {
            case ina::query_model::Parameter::eConstant:
            {
                if(param.getValueType()=="String")
                    paramValue = std::stod(param.getValue());
                else
                    paramValue = std::stod(param.getValue());
                break;
            }
            case ina::query_model::Parameter::eFunction:
            {
                paramValue = eval(context, param.getFunction(), getValueCallback);
                break;
            }
            case ina::query_model::Parameter::eMember:
            {
                if(getValueCallback == nullptr)
                    throw std::runtime_error("Missing callback to evaluate Member");
                getValueCallback(context, param.getName(), paramValue);
                break;
            }
            case ina::query_model::Parameter::eUndefined:
                throw std::runtime_error("eUndefined parameter type");
            default:
                throw std::runtime_error("Unkown parameter type");
        }

        return paramValue;
    }

	size_t getDeps(const ina::query_model::Parameter& param, std::vector<std::string>& deps)
    {
        size_t nbOfDeps = 0;
        switch (param.getType())
        {
            case ina::query_model::Parameter::eConstant:
                break;
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

        return nbOfDeps;
    }

    Parameter::~Parameter()
    {
        m_type = eUndefined;
    }
    
    Parameter::eType Parameter::getType() const
    {
        return m_type;
    }

    const std::string & Parameter::getValueType() const
    {
        return m_valueType;
    }

    const std::string & Parameter::getName() const
    {
        return m_name;
    }

    const std::string & Parameter::getValue() const
    {
        return m_value;
    }

    const Function& Parameter::getFunction() const
    {
        if(m_function == nullptr)
            throw TRACED_InA_EXCEPTION("invalid call to getFunction");
        return *m_function;
    }

}
