#pragma once

#include <string>

class JSONGenericObject; // From <json/jsonReader.h>

namespace ina::query_model
{
	class Dimension;

	class Attribute;
	void read(Attribute& obj, const JSONGenericObject& jsonNode);

	class Attribute
	{
	public:
		Attribute() = default;

		const std::string&   getName() const;
		const std::string&   getObtainability() const;

	private:
		std::string                 m_name;
		std::string                 m_Obtainability;
		
		friend void read(Attribute& obj, const JSONGenericObject& jsonNode);
		friend void read(Dimension& obj, const JSONGenericObject& jsonNode);
	};

} // namespace
