#pragma once

#include <string>
#include <memory>

class JSONGenericObject; // From <json/jsonReader

namespace ina::query_model
{
	class Parameter;

	class Function;    // Function.h
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
