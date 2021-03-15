#pragma once

#include <string>
#include <string_view>

#include <vector>
#include <variant>
#include <functional>

#include <memory>

#include <cmath>
#include <charconv>   // from_chars



namespace dbproxy
{
	class Value : protected std::string_view
	{
		public:
		Value(const char* str) : std::string_view(str) {}
		
		const std::string_view& getStringView() const {return (*this);};
		//double                  getDouble() const {double dble;std::from_chars(data(), data() + size(), dble);return dble;};
		const std::string       getString() const {return std::string(static_cast<const std::string_view&>(*this));};
	};

	class Row : public std::vector<Value>
	{
	public:
		Row() = default;
		Row(std::initializer_list<Value> vals) : std::vector<Value>(vals) {};
	};

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

	class DBProxy
	{
	public:
		static std::shared_ptr<DBProxy> getDBProxy(const std::string& cnxString);
		virtual ~DBProxy();

		std::vector<TableDescr>&       getTables();
		const std::vector<TableDescr>& getTables() const;
		const TableDescr&              getTableDescr(const std::string& name) const;

		virtual void  executeSQL(const std::string& SQL, const std::function<void (Row const&)>* calback) const = 0;

	protected:
		DBProxy();

	private:
		std::vector<TableDescr>  m_tablesDescr;
	};

} // dbproxy
