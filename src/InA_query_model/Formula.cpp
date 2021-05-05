#include "Formula.h"

namespace ina::query_model
{
    Formula::Formula(const Function& _function):
    m_function(_function)
    {

    }

    const Function& Formula::getFunction() const
    {
        return m_function;
    }    

}
