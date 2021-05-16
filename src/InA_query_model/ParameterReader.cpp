#include "Parameter.h"
#include "Function.h"
#include <json/jsonReader.h>    // For JSONGenericObject
#include <memory>
#include <stdexcept>

namespace ina::query_model
{
	void read(Parameter& param, const JSONGenericObject& node)
	{
        param.m_function = nullptr;
        if(JSONGenericObject constObj = node.getObject("Constant"))
        {
            param.m_type = Parameter::eConstant;
            param.m_value = constObj.getString("Value");
            param.m_valueType = constObj.getString("ValueType");
        }
        else if(JSONGenericObject memberObj = node.getObject("Member"))
        {
            param.m_type = Parameter::eMember;
            param.m_value = memberObj.getString("Name");

        }    
        else if(JSONGenericObject functionObj = node.getObject("Function"))
        {
            param.m_type = Parameter::eFunction;
            
            param.m_function = std::make_shared<Function>();
            
            read(*(param.m_function), functionObj);
            
        }    
    }
}