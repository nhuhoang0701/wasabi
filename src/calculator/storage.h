#pragma once

#include <dbproxy/dbproxy.h>

#include <tuple>
#include <string>
#include <map>
#include <vector>
#include <variant>

namespace calculator
{
	typedef std::variant<std::string, double> Value;

	enum class eDataType {Number, String};
	enum class eColumnType {Indexed, NoneIndexed};

	class ColumnNoneIndexed : public std::vector<Value>
	{
		public:
		size_t getNbDistinctVals() const {return std::vector<Value>::size();}
	};

	typedef size_t Index;
	class ColumnIndexed : protected std::vector<Index>
	{
		public:
		size_t getNbDistinctVals() const {return m_values.size();}
		size_t size() const {return std::vector<Index>::size();}
		
		const Value& operator[](size_t index) const {return m_values[std::vector<Index>::operator [](index)];}

		void push_back(const Value& value)
		{
			Index index;
			if(m_idxByVal.find(value) != m_idxByVal.end())
			{
				index = m_idxByVal[value];
			}
			else
			{
				index = m_values.size();
				m_idxByVal[value] = index;
				m_values.push_back(value);
			}

			std::vector<Index>::push_back(index);
		}

		std::vector<Value>      m_values;
		std::map<Value, Index>  m_idxByVal;
	};

	class DataStorage
	{
	public:
		typedef std::tuple<eDataType, eColumnType, std::variant<ColumnIndexed, ColumnNoneIndexed>> Column;
		
		std::vector<Column>  m_cols;

	public:
		void addColumn(eDataType dt, eColumnType type)
		{
			if(type == eColumnType::Indexed)
				m_cols.emplace_back(dt, type, ColumnIndexed());
			else if(type == eColumnType::NoneIndexed)
				m_cols.emplace_back(dt, type, ColumnNoneIndexed());
			else
				throw std::runtime_error("unknow column type");
		}

		size_t        size() const {return m_cols.size();}
		const Column& operator[](size_t index) const {return m_cols[index];}

		void    insertRow(const dbproxy::Row& row)
		{
			if(row.size() != m_cols.size())
				throw std::runtime_error("row size didn't match column size");

			for(size_t i = 0; i < m_cols.size(); ++i)
			{
				auto& col = std::get<2>(m_cols[i]);
				const eColumnType type = std::get<1>(m_cols[i]);
				const eDataType dt = std::get<0>(m_cols[i]);

				if(type == eColumnType::Indexed)
					std::get<ColumnIndexed>(col).push_back(row[i]);
				else if(type == eColumnType::NoneIndexed)
					std::get<ColumnNoneIndexed>(col).push_back(std::stod(row[i]));
				else
					throw std::runtime_error("unknow column type");

			}
		}
	};
} // cube
