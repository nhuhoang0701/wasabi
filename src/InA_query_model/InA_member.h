#pragma once

#include <string>

class JSONGenericObject;    // #include <json/jsonReader.h>

namespace ina::query_model
{
	class Dimension;

	class InA_member;
	void read(InA_member& obj, const JSONGenericObject& jsonNode);

	typedef std::string Aggregation;
	class InA_member
	{
		public:
			InA_member(const std::string & name, const Aggregation & aggregation);
			const std::string& getName() const;

			const Aggregation & getAggregation() const;

		private:
			InA_member() = default;

			std::string _name; 
			Aggregation _aggregation;

			friend void read(InA_member& obj, const JSONGenericObject& jsonNode);

			friend void read(Dimension& obj, const JSONGenericObject& jsonNode);
	};
}