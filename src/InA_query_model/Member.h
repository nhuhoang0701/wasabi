#pragma once

#include <string>
#include "Formula.h"

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
			enum  eMemberType {eAggregation=0, eFormula};
			Member(const std::string & name, const Aggregation & aggregation);

			Member(const std::string & name, const Formula & formula);

			const std::string& getName() const;

			const Aggregation & getAggregation() const;

			const query_model::Formula & getFormula() const;

			const eMemberType getType() const;

		private:
			Member() = default;

			std::string _name; 
			Aggregation _aggregation;
			Formula 	_formula;
			eMemberType _type = eAggregation;

			friend void read(Member& obj, const JSONGenericObject& jsonNode);

			friend void read(Dimension& obj, const JSONGenericObject& jsonNode);
	};
}