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

	Member::Member(Member&& other)
	: m_memberOperand(std::move(other.m_memberOperand)),
	  m_formula(std::move(other.m_formula)),
	  m_name(other.m_name),
	  m_description(other.m_description),
	  m_aggregation(other.m_aggregation)
	{
	}

	Member::Member(const Member& other)
	: m_memberOperand(other.m_memberOperand!=nullptr?new MemberOperand(*other.m_memberOperand):nullptr),
	  m_formula(other.m_formula!=nullptr?new Formula(*other.m_formula):nullptr),
	  m_name(other.m_name),
	  m_description(other.m_description),
	  m_aggregation(other.m_aggregation)
	{
	}

	Member& Member::operator =(const Member& other)
	{
		m_memberOperand.reset(other.m_memberOperand!=nullptr?new MemberOperand(*other.m_memberOperand):nullptr);
		m_formula.reset(other.m_formula!=nullptr?new Formula(*other.m_formula):nullptr);
		m_name = other.m_name;
		m_description = other.m_description;
		m_aggregation = other.m_aggregation;

		return *this;
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
