#pragma once

#include <dbproxy/dbproxy.h>  // For dbproxy::Row

#include <common/data.h>
#include <common/Log.h>

#include <memory>
#include <stdexcept>
#include <tuple>
#include <string>
#include <map>
#include <vector>


namespace calculator
{
	enum class eColumnType {Indexed, NoneIndexed};

	class ColumnData
	{
	public:
		ColumnData(const std::string& name, common::eDataType dt)
		: m_name(name), m_datatype(dt) {}

		common::eDataType    getDataType() const {return m_datatype;}
		const std::string    getName() const {return m_name;}

		virtual void push_back(const common::Value& value) = 0;

		virtual size_t               getNumberOfValues() const = 0;
		virtual const common::Value& getValueAtValueIdx(size_t valueIndex) const = 0;

		virtual size_t               getRowCount() const = 0;
		virtual const common::Value& getValueAtRowIdx(size_t rowIndex) const = 0;
		virtual size_t               getValueIndexFromRowIdx(size_t rowIndex) const = 0;

	private:
		const std::string  m_name;
		const common::eDataType    m_datatype;
	};
	class ColumnNoneIndexed : public ColumnData
	{
	public:
		ColumnNoneIndexed(const std::string& name, common::eDataType dt)
			: ColumnData(name, dt) {}

		void push_back(const common::Value& value)
		{
			m_values.push_back(value);
		}

		virtual size_t               getNumberOfValues() const {return m_values.size();}
		virtual const common::Value& getValueAtValueIdx(size_t valueIndex) const {return m_values[valueIndex];}

		size_t                 getRowCount() const {return m_values.size();}
		const common::Value&   getValueAtRowIdx(size_t rowIndex) const {return m_values[rowIndex];}
		virtual size_t         getValueIndexFromRowIdx(size_t rowIndex) const {return rowIndex;}

	private:
		std::vector<common::Value> m_values;
	};

	class ColumnIndexed : public ColumnData
	{
	typedef size_t Index;
	public:
		ColumnIndexed(const std::string& name, common::eDataType dt)
			: ColumnData(name, dt) {}

		virtual size_t               getNumberOfValues() const {return m_values.size();}
		virtual const common::Value& getValueAtValueIdx(size_t valueIndex) const {return m_values[valueIndex];}

		size_t                 getRowCount() const {return m_valueIndexes.size();}		
		const common::Value&   getValueAtRowIdx(size_t rowIndex) const {return m_values[m_valueIndexes[rowIndex]];}
		virtual size_t         getValueIndexFromRowIdx(size_t rowIndex) const {return m_valueIndexes[rowIndex];}

		void push_back(const common::Value& value)
		{
			Index index = 0;
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

			m_valueIndexes.push_back(index);
		}

	private:
		std::vector<Index>              m_valueIndexes;
		std::vector<common::Value>      m_values;
		std::map<common::Value, Index>  m_idxByVal;
	};

	class DataStorage
	{
	public:
		DataStorage() = default;

		void clear() {m_rowsNb=0;m_cols.clear();m_colsIdxByName.clear();}

		bool          haveCol(const std::string& col_name) const {return m_colsIdxByName.find(col_name) != m_colsIdxByName.end();}
		size_t        getColIndex(const std::string& col_name) const;

		size_t                             getColumnCount() const {return m_cols.size();}
		const std::shared_ptr<ColumnData>& getColumn(const std::string& col_name) const {return m_cols[getColIndex(col_name)];}
		const std::shared_ptr<ColumnData>& getColumn(size_t index) const {return m_cols[index];}

		bool          haveData() const {return m_rowsNb!=0;}
		size_t        getRowCount() const {return m_rowsNb;}

		void addColumn(const std::string& name, common::eDataType dt, eColumnType type)
		{
			if(name.empty())
				throw std::runtime_error("Column with empty name not supported.");
			if(m_colsIdxByName.find(name) != m_colsIdxByName.end())
				throw std::runtime_error("Column already exist:" + name);

			std::shared_ptr<ColumnData> colData;
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
			//ScopeLog sc("DataStorage::insertRow");
			if(row.size() != m_cols.size())
				throw std::runtime_error("row size didn't match column size input:" + std::to_string(row.size()) + " expected" + std::to_string(m_cols.size()));

			// Ignore first line which have the column name
			if(m_cols[0]->getRowCount()==0 && row[0].getString() == m_cols[0]->getName())
				return;
			size_t idx = 0;
			for(auto& colData : m_cols)
			{
				const common::eDataType dt = colData->getDataType();
				//Logger::debug("row[idx].getString()", row[idx].getString());
				if(dt == common::eDataType::String)
					colData->push_back(common::Value(row[idx].getString()));
				else if(dt == common::eDataType::Numeric)
					colData->push_back(common::Value(std::stod(row[idx].getString())));
				else
					throw std::runtime_error("unknow column type");
				idx++;
			}
			m_rowsNb++;
		}

	private:
		std::map<std::string, size_t /*index*/>   m_colsIdxByName;
		std::vector<std::shared_ptr<ColumnData>>  m_cols;

		size_t               m_rowsNb = 0;
	};
} // cube
