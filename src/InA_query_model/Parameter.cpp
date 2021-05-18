#include "Parameter.h"
#include "Function.h"
#include <cstddef>
#include <exceptions/InA_Exception.h>

namespace ina::query_model
{
    Parameter::Parameter(Parameter::eType type, const std::string& value):
    m_type(type),
    m_value(value),
    m_function(nullptr)
    {

    }
    
    Parameter::~Parameter()
    {        
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
