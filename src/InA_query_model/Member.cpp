#include "Member.h"

#include <json/jsonReader.h>

namespace ina::query_model
{
	Member::Member(Member&& other)
	: m_formula(std::move(other.m_formula)),
	  m_name(other.m_name),
	  m_description(other.m_description),
	  m_aggregation(other.m_aggregation)
	{
	}

	Member::Member(const Member& other)
	: m_formula(other.m_formula!=nullptr?new Formula(*other.m_formula):nullptr),
	  m_name(other.m_name),
	  m_description(other.m_description),
	  m_aggregation(other.m_aggregation)
	{
	}

	Member& Member::operator =(const Member& other)
	{
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
}
