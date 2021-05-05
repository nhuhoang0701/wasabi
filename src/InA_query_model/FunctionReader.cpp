#include "Function.h"
#include "InA_query_model/Parameter.h"
#include "InA_query_model/Query.h"

#include <json/jsonReader.h>    // For JSONGenericObject
#include <stdexcept>

namespace ina::query_model
{
	void read(Function& function, const JSONGenericObject& functionNode)
	{
        function.m_name = functionNode.getString("Name");
        if(function.m_name == "+")
        {
            function.m_type = Function::eAdd;
        }
        else if(function.m_name == "-")
        {
            function.m_type = Function::eSubtract;
        }
        else if(function.m_name == "*")
        {
            function.m_type = Function::eMultipy;
        }
        else if(function.m_name == "/")
        {
            function.m_type = Function::eDivide;
        }
        else if(function.m_name == "decfloat")
        {
            function.m_type = Function::eDecFloat;
        }
        else if(function.m_name == "**")
        {
            function.m_type = Function::eSquare;
        }
        
        if(const auto& params = functionNode.getArray("Parameters"))
		{
			for(int i = 0;i < params.size();i++)
			{
				query_model::Parameter parameter;
                read(parameter, params[i]);
                function.addParameter(parameter);
			}
		}
        
        
    }
}