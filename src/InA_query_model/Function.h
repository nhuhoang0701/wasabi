#pragma once

//#include "Member.h"
#include "Parameter.h"
#include <string>
#include <vector>
#include <map>

class JSONGenericObject; // From <json/jsonReader.h>

namespace ina::query_model
{
	
   	class Definition;

	class Function;
	class Parameter;
	void read(Function& obj, const JSONGenericObject& jsonNode);
	//void write(const Function& obj, JSONWriter& jsonNode);

	class Function
	{
		public:
			
			Function() = default;

			enum  eFunctionType {eAdd=0, eSubtract, eMultipy, eDivide, eDecFloat, eSquare};
			Function(const std::string& name, const std::string& desc, eFunctionType type, const std::vector<Parameter>& params);
			
			

			const std::string & getName() const;
			const std::string & getDescription() const;
			void  addParameter(const Parameter& param);

			eFunctionType       getFunctionType() const;

			size_t 				getChildrenCount() const;

			const Parameter&	getChild(size_t index) const;
		private:
			std::vector<Parameter> m_vParams;
			eFunctionType		   m_type;
			std::string			   m_name;
			std::string			   m_description;

			friend void read(Function& obj, const JSONGenericObject& jsonNode);

	};

}