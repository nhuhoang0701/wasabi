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
			Member() = default;
			Member(const Member& other);
			Member(Member&& other);
			
			Member& operator =(const Member& other);

			const std::string& getName() const;

			const Aggregation & getAggregation() const;

			const Formula*     getFormula() const;

		private:
			std::unique_ptr<Formula>  m_formula;
			std::string m_name; 
			std::string m_description; 
			Aggregation m_aggregation;

			friend void read(Member& obj, const JSONGenericObject& jsonNode);

			friend void read(Dimension& obj, const JSONGenericObject& jsonNode);
	};
}