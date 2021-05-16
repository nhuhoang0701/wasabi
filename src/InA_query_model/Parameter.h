#pragma once

#include <string>
#include <vector>
#include <memory>

class JSONGenericObject; // From <json/jsonReader.h>

namespace ina::query_model
{	
	class Function;
	class Parameter
	{
		public:
			
			enum eType{eUndefined=-1, eMember=0, eFunction, eConstant};
			Parameter() = default;
			~Parameter();

			Parameter(eType type, const std::string& value);

			eType  getType() const;

			const std::string& getValueType() const;
			const std::string& getValue() const;
			const Function&    getFunction() const;
		private:
			eType  m_type = eUndefined;
			std::string m_value;
			std::string m_valueType;
			std::shared_ptr<Function> m_function = nullptr;

			friend void read(Parameter& obj, const JSONGenericObject& jsonNode);
	};
}
