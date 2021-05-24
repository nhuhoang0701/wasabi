#pragma once

#include <string>
#include <variant>

namespace common
{
	typedef std::variant<std::string, double> Value;

	enum class eDataType {Uniti = -1, Numeric=0, String, DateTime};
}