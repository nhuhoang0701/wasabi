#include "Parameter.h"
#include "Function.h"

#include <exceptions/InA_Exception.h>

namespace ina::query_model
{
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
