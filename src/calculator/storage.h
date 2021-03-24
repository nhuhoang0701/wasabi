#pragma once

#include <dbproxy/dbproxy.h>

#include <memory>
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

	class ColumnData
	{
	public:
		ColumnData(const std::string& name)
		: m_name(name) {}

		virtual void push_back(const Value& value) = 0;

		virtual size_t getNbVals() const = 0;
		virtual size_t getNbDistinctVals() const = 0;

		virtual const Value& operator[](size_t index) const = 0;

	private:
		const std::string  m_name;
	};
	class ColumnNoneIndexed : public ColumnData
	{
	public:
		ColumnNoneIndexed(const std::string& name)
			: ColumnData(name) {}

		void push_back(const Value& value)
		{
			m_data.push_back(value);
		}

		size_t getNbVals() const {return m_data.size();}
		size_t getNbDistinctVals() const {return m_data.size();}

		const Value& operator[](size_t index) const {return m_data[index];}

	private:
		std::vector<Value> m_data;
	};

	typedef size_t Index;
	class ColumnIndexed : public ColumnData
	{
	public:
		ColumnIndexed(const std::string& name)
			: ColumnData(name) {}

		size_t getNbVals() const {return m_indexes.size();}
		size_t getNbDistinctVals() const {return m_values.size();}
		
		const Value& operator[](size_t index) const {return m_values[m_indexes[index]];}

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

			m_indexes.push_back(index);
		}

	private:
		std::vector<Index>      m_indexes;
		std::vector<Value>      m_values;
		std::map<Value, Index>  m_idxByVal;
	};

	class DataStorage
	{
	public:
		typedef std::tuple<eDataType, eColumnType, std::shared_ptr<ColumnData>> Column;

	public:
		DataStorage() = default;

		bool          haveCol(const std::string& col_name) const {return m_colsIdxByName.find(col_name) != m_colsIdxByName.end();}
		size_t        getColIndex(const std::string& col_name) const {return m_colsIdxByName.at(col_name);}

		size_t        getColNbrs() const {return m_cols.size();}
		const Column& operator[](size_t index) const {return m_cols[index];}

		size_t        getRowNbrs() const {return m_rowsNb;}

		void addColumn(const std::string name, eDataType dt, eColumnType type)
		{
			if(m_colsIdxByName.find(name) != m_colsIdxByName.end())
				throw std::runtime_error("Column already exist:" + name);

			std::shared_ptr<ColumnData> colData;
			if(type == eColumnType::Indexed)
				colData = std::make_shared<ColumnIndexed>(name);
			else if(type == eColumnType::NoneIndexed)
				colData = std::make_shared<ColumnNoneIndexed>(name);
			else
				throw std::runtime_error("unknow column type");
			m_cols.emplace_back(dt, type, colData);

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

				if(dt == eDataType::String)
					colData->push_back(row[idx].getString());
				else if(dt == eDataType::Number)
					colData->push_back(std::stod(row[idx].getString()));
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
