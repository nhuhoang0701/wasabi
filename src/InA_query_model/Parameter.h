#pragma once

#include <string>
#include <vector>
#include <memory>

class JSONGenericObject; // From <json/jsonRe

#include <calculator/common.h>   // For calculator::Valueader.h>

namespace ina::query_model
{
	class Parameter;

	calculator::Value eval(const void* context, const ina::query_model::Parameter& param, void (*getValueCallback)(const void* context, const std::string& nameObj, calculator::Value& value));
	size_t getDeps(const ina::query_model::Parameter& param, std::vector<std::string>& deps);

	class Function;
	class Parameter
	{
		public:
			
			enum eType{eUndefined=-1, eMember=0, eFunction, eConstant};
			Parameter() = default;
			~Parameter();

			eType  getType() const;

			// For Member
			const std::string& getName() const;

			// For Constant
			const std::string& getValueType() const;
			const std::string& getValue() const;

			// For Function
			const Function&    getFunction() const;
		private:
			eType  m_type = eUndefined;
			std::string m_name;
			std::string m_value;
			std::string m_valueType;
			std::shared_ptr<Function> m_function = nullptr;

			friend void read(Parameter& obj, const JSONGenericObject& jsonNode);
	};
}
