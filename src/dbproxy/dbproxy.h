#pragma once

#include <string>
#include <vector>
#include <variant>
#include <functional>

#include <memory>

#include <cmath>



namespace dbproxy
{
	class ColumnDescr
	{
	public:
		ColumnDescr(const std::string& name, const std::string& datatype) :
			m_name(name),
			m_datatype(datatype)
		{
		};
		bool operator == (const ColumnDescr& rhs) const;

		const std::string&  getName() const {return m_name;};
		const std::string&  getDataType() const {return m_datatype;};

	private:
		const std::string  m_name;
		const std::string  m_datatype;
	};

	class TableDescr : public std::vector<ColumnDescr> 
	{
		friend class DBProxy;

	public:
		bool operator == (const TableDescr& rhs) const;

		void setName(const std::string& name) {m_name = name;};

		const std::string&               getName() const {return m_name;};
		const std::vector<ColumnDescr>&  getColumnsDescr() const {return *this;};

	private:
		std::string               m_name;
	};

	class Value : public std::string
	{
	public:
		Value(const std::string& str) : std::string(str) {}
		
		const std::string& getString() const {return (*this);};
	};

	class Row : public std::vector<Value>
	{
	public:
	};

	class DBProxy
	{
	public:
		static std::shared_ptr<DBProxy> getDBProxy(const std::string& cnxString);
		virtual ~DBProxy();

		std::vector<TableDescr>&       getTables();
		const std::vector<TableDescr>& getTables() const;
		const TableDescr&              getTableDescr(const std::string& name) const;

		virtual void  executeSQL(const std::string& SQL, std::function<void (Row const&)> calback) const = 0;

	protected:
		DBProxy();

	private:
		std::vector<TableDescr>  m_tablesDescr;
	};

} // dbproxy
