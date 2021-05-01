#pragma once

#include <dbproxy/dbproxy.h>  // For dbproxy::Row

#include <memory>
#include <stdexcept>
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
		ColumnData(const std::string& name, eDataType dt)
		: m_name(name), m_datatype(dt) {}

		eDataType            getDataType() const {return m_datatype;}
		const std::string    getName() const {return m_name;}

		virtual void push_back(const Value& value) = 0;

		virtual size_t       getNumberOfValues() const = 0;
		virtual const Value& getValueAtValueIdx(size_t valueIndex) const = 0;

		virtual size_t       getRows() const = 0;
		virtual const Value& getValueAtRowIdx(size_t rowIndex) const = 0;
		virtual size_t       getValueIndexFromRowIdx(size_t rowIndex) const = 0;

	private:
		const std::string  m_name;
		const eDataType    m_datatype;
	};
	class ColumnNoneIndexed : public ColumnData
	{
	public:
		ColumnNoneIndexed(const std::string& name, eDataType dt)
			: ColumnData(name, dt) {}

		void push_back(const Value& value)
		{
			m_values.push_back(value);
		}

		virtual size_t       getNumberOfValues() const {return m_values.size();}
		virtual const Value& getValueAtValueIdx(size_t valueIndex) const {return m_values[valueIndex];}

		size_t         getRows() const {return m_values.size();}
		const Value&   getValueAtRowIdx(size_t rowIndex) const {return m_values[rowIndex];}
		virtual size_t getValueIndexFromRowIdx(size_t rowIndex) const {return rowIndex;}

	private:
		std::vector<Value> m_values;
	};

	class ColumnIndexed : public ColumnData
	{
	typedef size_t Index;
	public:
		ColumnIndexed(const std::string& name, eDataType dt)
			: ColumnData(name, dt) {}

		virtual size_t       getNumberOfValues() const {return m_values.size();}
		virtual const Value& getValueAtValueIdx(size_t valueIndex) const {return m_values[valueIndex];}

		size_t         getRows() const {return m_rowIndexes.size();}		
		const Value&   getValueAtRowIdx(size_t rowIndex) const {return m_values[m_rowIndexes[rowIndex]];}
		virtual size_t getValueIndexFromRowIdx(size_t rowIndex) const {return m_idxByVal.at(m_values[m_rowIndexes[rowIndex]]);}

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

			m_rowIndexes.push_back(index);
		}

	private:
		std::vector<Index>      m_rowIndexes;
		std::vector<Value>      m_values;
		std::map<Value, Index>  m_idxByVal;
	};

	class DataStorage
	{
	public:
		typedef std::shared_ptr<ColumnData> Column;

	public:
		DataStorage() = default;

		void clear() {m_rowsNb=0;m_cols.clear();m_colsIdxByName.clear();}

		bool          haveCol(const std::string& col_name) const {return m_colsIdxByName.find(col_name) != m_colsIdxByName.end();}
		size_t        getColIndex(const std::string& col_name) const {return m_colsIdxByName.at(col_name);}

		size_t        getColNbrs() const {return m_cols.size();}
		const Column& getColumn(const std::string& col_name) const {return m_cols[getColIndex(col_name)];}
		const Column& getColumn(size_t index) const {return m_cols[index];}

		size_t        getRowNbrs() const {return m_rowsNb;}

		void addColumn(const std::string name, eDataType dt, eColumnType type)
		{
			if(m_colsIdxByName.find(name) != m_colsIdxByName.end())
				throw std::runtime_error("Column already exist:" + name);

			Column colData;
			if(type == eColumnType::Indexed)
				colData = std::make_shared<ColumnIndexed>(name, dt);
			else if(type == eColumnType::NoneIndexed)
				colData = std::make_shared<ColumnNoneIndexed>(name, dt);
			else
				throw std::runtime_error("unknow column type");
			m_cols.emplace_back(colData);

			m_colsIdxByName[name] = m_cols.size()-1;
		}

		void    insertRow(const dbproxy::Row& row)
		{
			if(row.size() != m_cols.size())
				throw std::runtime_error("row size didn't match column size input:" + std::to_string(row.size()) + " expected" + std::to_string(m_cols.size()));

			size_t idx = 0;
			for(auto& colData : m_cols)
			{
				const eDataType dt = colData->getDataType();

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
		std::map<std::string, size_t /*index*/> m_colsIdxByName;
		std::vector<Column>  m_cols;

		size_t               m_rowsNb = 0;
	};
} // cube
