#pragma once

#include <string>
#include <variant>

namespace common
{
	enum class eDataType {Undefined = -1, Numeric=0, String, DateTime};

	class Value
	{
		public:
		Value() {};
		explicit Value(const double& val) : m_data(val) {};
		explicit Value(const std::string& val) : m_data(val) {};

		Value& operator =(const Value& rhs) { m_data = rhs.m_data; return *this;};
		Value& operator =(const std::string& rhs) { m_data = rhs; return *this;};
		Value& operator =(const double& rhs) { m_data = rhs; return *this;};

		bool operator ==(const Value& rhs) const { return m_data == rhs.m_data;};
		    
		bool isDouble() const {return std::holds_alternative<double>(m_data);}
		double getDouble() const
		{
			if(std::holds_alternative<double>(m_data))
			{
				return std::get<double>(m_data);
			}
			throw std::runtime_error("getDouble: Not a double");
		}
		
		bool isString() const {return std::holds_alternative<std::string>(m_data);}
		const std::string getString() const
		{
			if(std::holds_alternative<std::string>(m_data))
			{
				return std::get<std::string>(m_data);
			}
			throw std::runtime_error("getString: Not a string");
		}

		private:
		friend bool operator<(const Value& lhs, const Value& rhs );
		 std::variant<std::string, double> m_data;
	};
	inline bool operator<(const Value& lhs, const Value& rhs )
	{
		return ( lhs.m_data < rhs.m_data );
	}
	
    inline std::ostream& operator<<(std::ostream& stream, const Value& value)
	{
		if(value.isDouble())
		{
			stream << std::to_string(value.getDouble());
		}
		else if(value.isString())
		{
			stream << value.getString();
		}
		else
			throw std::runtime_error("common::Value: Not a double nor a string");
		return stream;
	}

}