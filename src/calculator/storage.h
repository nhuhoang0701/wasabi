#pragma once

#include <dbproxy/dbproxy.h>

#include <stdexcept>
#include <string_view>
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

template <typename STORAGE_TYPE>
	class Column : public std::vector<STORAGE_TYPE>
	{
	public:
		Column(const std::string& name)
		: m_name(name) {}

	private:
		const std::string  m_name;
	};
	class ColumnNoneIndexed : public Column<Value>
	{
	public:
		ColumnNoneIndexed(const std::string& name)
			: Column(name) {}

		size_t getNbDistinctVals() const {return std::vector<Value>::size();}

	private:
	};

	typedef size_t Index;
	class ColumnIndexed : protected Column<Index>
	{
	public:
		ColumnIndexed(const std::string& name)
			: Column(name) {}

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

	public:
		DataStorage() = default;

		size_t        getColIndexOf(const std::string& col_name) const {return m_colsIdxByName.at(col_name);}

		size_t        getColNbrs() const {return m_cols.size();}
		const Column& operator[](size_t index) const {return m_cols[index];}

		size_t        getRowNbrs() const {return m_rowsNb;}

		void addColumn(const std::string& name, eDataType dt, eColumnType type)
		{
			if(m_colsIdxByName.find(name) != m_colsIdxByName.end())
				throw std::runtime_error("Column already exist:" + name);

			if(type == eColumnType::Indexed)
				m_cols.emplace_back(dt, type, ColumnIndexed(name));
			else if(type == eColumnType::NoneIndexed)
				m_cols.emplace_back(dt, type, ColumnNoneIndexed(name));
			else
				throw std::runtime_error("unknow column type");

			m_colsIdxByName[name] = m_cols.size()-1;
		}

		void    insertRow(const dbproxy::Row& row)
		{
			if(row.size() != m_cols.size())
				throw std::runtime_error("row size didn't match column size input:" + std::to_string(row.size()) + " expected" + std::to_string(m_cols.size()));

			size_t idx = 0;
			for(auto& col : m_cols)
			{
				auto& colData = std::get<2>(col);
				const eColumnType type = std::get<1>(col);
				const eDataType dt = std::get<0>(col);

				if(type == eColumnType::Indexed)
					std::get<ColumnIndexed>(colData).push_back(row[idx].getString());
				else if(type == eColumnType::NoneIndexed)
					std::get<ColumnNoneIndexed>(colData).push_back(std::stod(row[idx].getString()));
				else
					throw std::runtime_error("unknow column type");
				idx++;
			}
			m_rowsNb++;
		}

	private:
		std::map<std::string_view, size_t /*index*/> m_colsIdxByName;
		std::vector<Column>  m_cols;

		size_t               m_rowsNb = 0;
	};
} // cube
