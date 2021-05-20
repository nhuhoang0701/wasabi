#pragma once

#include "Parameter.h"
#include <string>
#include <vector>
#include <map>

class JSONGenericObject; // From <json/jsonReader.h>

#include <calculator/common.h>   // For calculator::Value

namespace ina::query_model
{
	
   	class Definition;

	class Function;
	class Parameter;
	void read(Function& obj, const JSONGenericObject& jsonNode);
	//void write(const Function& obj, JSONWriter& jsonNode);

	calculator::Value eval(const void* context, const ina::query_model::Function& fct, void (*getValueCallback)(const void* context, const std::string& nameObj, calculator::Value& value));
	size_t getDeps(const ina::query_model::Function& fct, std::vector<std::string>& deps);

	class Function
	{
		public:
			
			Function() = default;

			enum  eFunctionType
			{
				eUndefined=-1,
				eDecFloat=0,
				eAdd,
				eSubtract,
				eMultipy,
				eDivide,
				eSquare,
				eAnd,
				eOr,
				eNot,
				eNotEqual,
				eEqual,
				eLess,
				eLessEq,
				eGreater,
				eGreaterEq,
				eIf
				};
			Function(const std::string& name, const std::string& desc, eFunctionType type, const std::vector<Parameter>& params);
			
			const std::string & getName() const;
			const std::string & getDescription() const;

			eFunctionType       getFunctionType() const;

			void                addParameter(const Parameter& param);
			size_t 				getParameterCount() const;
			const Parameter&	getParameter(size_t index) const;
		private:
			std::vector<Parameter> m_vParams;
			eFunctionType		   m_type = eUndefined;
			std::string			   m_name;
			std::string			   m_description;

			friend void read(Function& obj, const JSONGenericObject& jsonNode);
	};
}
