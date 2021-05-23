#pragma once

#include <memory>
#include <string>
#include "Formula.h"

class JSONGenericObject;    // #include <json/jsonReader.h>

namespace ina::query_model
{
	class Member;

	class MemberOperand
	{
		public:
			const std::string&  getValue() const;
		private:
		std::string m_attributeName;
		std::string m_value;
		std::string m_comparison;
		
		friend void read(Member& obj, const JSONGenericObject& jsonNode);
	};
	class Dimension;
	
	void read(Member& obj, const JSONGenericObject& jsonNode);
	typedef std::string Aggregation;
	class Member
	{
		public:
			// TODO: Wait intermediate model on top of the InA query and the InAcube
			static const std::string& getName(const Member& member);

			Member() = default;

			const std::string&   getName() const;
			const std::string&   getDescription() const;
			const Aggregation&   getAggregation() const;

			const Formula*       getFormula() const;
			const MemberOperand* getMemberOperand() const;

		private:
			std::shared_ptr<MemberOperand> m_memberOperand;
			std::shared_ptr<Formula>       m_formula;
			std::string m_name; 
			std::string m_description; 
			Aggregation m_aggregation;

			friend void read(Member& obj, const JSONGenericObject& jsonNode);

			friend void read(Dimension& obj, const JSONGenericObject& jsonNode);
	};
}