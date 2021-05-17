#pragma once

#include <string>
#include <variant>

namespace calculator
{
	typedef std::variant<std::string, double> Value;

	enum class eDataType {Number, String};
}