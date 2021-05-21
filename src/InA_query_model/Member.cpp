#include "Member.h"

#include <json/jsonReader.h>

namespace ina::query_model
{
	std::string Member::getName(const Member& member)
	{
		/*
		"MemberOperand": {
		"Comparison": "=",
		"AttributeName": "Measures",
		"Value": "SIGNEDDATA"
		}
		*/	
		
		/*
		"MemberOperand": {
		"Comparison": "=",
		"AttributeName": "YEAR",
		"Value": "2015"
		}
		*/
		std::string name;
		if(member.getMemberOperand() != nullptr)
			name = member.getMemberOperand()->getValue();
		else
			name = member.getName();

		return name;
	}
	const std::string&  MemberOperand::getValue() const
	{
		return m_value;
	}
	
	const std::string& Member::getName() const
	{
		return m_name;
	}
	
	const Aggregation& Member::getAggregation() const
	{
		return m_aggregation;
	}

	const Formula* Member::getFormula() const
	{
		return m_formula.get();
	}

	const MemberOperand* Member::getMemberOperand() const
	{
		return m_memberOperand.get();
	}
}
