#pragma once

#include <string>

class JSONGenericObject;    // #include <json/jsonReader.h>

namespace ina::query_model
{
	class Dimension;

	class Member;
	void read(Member& obj, const JSONGenericObject& jsonNode);

	typedef std::string Aggregation;
	class Member
	{
		public:
			Member(const std::string & name, const Aggregation & aggregation);
			const std::string& getName() const;

			const Aggregation & getAggregation() const;

		private:
			Member() = default;

			std::string _name; 
			Aggregation _aggregation;

			friend void read(Member& obj, const JSONGenericObject& jsonNode);

			friend void read(Dimension& obj, const JSONGenericObject& jsonNode);
	};
}