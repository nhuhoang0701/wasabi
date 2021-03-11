#pragma once

#include <tuple>
#include <string>
#include <vector>
#include <variant>

namespace dbproxy {class Row;};

namespace cube
{
	class DataStorage
	{
		typedef std::variant<std::string, double> Value;

		enum class eDataType {Number, String};
		enum class eColumnType {Indexed, NoneIndexed};

		public:
		class ColumnNoneIndexed : private std::vector<Value>
		{
			size_t getDistinctVal() {return size();}
		};

		typedef size_t Index;
		class ColumnIndexed : private std::vector<Index>
		{
			size_t getDistinctVal() {return m_values.size();}

			void push_back(const Value& value)
			{
				Index index;
				if(m_idxByVal.find(value) != m_idxByVal.end())
					index = m_idxByVal[value];
				else
				{
					index = m_values.size();
					m_values.push_back(val);
				}

				push_back(index);
			}

			std::vector<Value>      m_values;
			std::map<Value, Index>  m_idxByVal;
		};
	};
} // cube
