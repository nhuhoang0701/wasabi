#pragma once

#include <string>
#include <vector>
#include <map>

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
		Attribute(    const std::string& name,
					  const std::string& Obtainability);

		~Attribute();

		const std::string&   getName() const;
		const std::string&   getObtainability() const;

		const Dimension&             getDimension() const;


	private:
		std::string                 m_name;
		std::string                 m_Obtainability;
		const Dimension*            m_dim;
		
		friend void read(Attribute& obj, const JSONGenericObject& jsonNode);

		friend void read(Dimension& obj, const JSONGenericObject& jsonNode);
	};

} // namespace
