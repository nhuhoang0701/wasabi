#pragma once

#include <string>
#include <vector>
#include <variant>
#include <functional>

#include <cmath>


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

class TableDescr
{
	friend class DBProxy;

public:
	bool operator == (const TableDescr& rhs) const;

	const std::string&               getName() const {return m_name;};
	const std::vector<ColumnDescr>&  getColumnsDescr() const {return m_columns;};

private:
	std::string               m_name;
	std::vector<ColumnDescr>  m_columns;
};

class Value : public std::variant<std::string, double>
{
public:
	Value(const std::string& str) : std::variant<std::string, double>(str) {}
	Value(double dble) : std::variant<std::string, double>(dble) {}
	
	const std::string& getString() const {return std::get<std::string>(*this);};
	const double&      getDouble() const {return std::get<double>(*this);};
};

class Row : public std::vector<Value>
{
public:
};

class DBProxy
{
public:
	static DBProxy getDBProxy(const std::string& cnxString);

	const std::vector<TableDescr>& getTables() const;
	const TableDescr& getTableDescr(const std::string& name) const;

	void  executeSQL(const std::string& SQL, std::function<void (Row const&)> calback)const;

private:
	DBProxy(const std::string& cnxString);

private:
	std::vector<TableDescr>  m_tablesDescr;
};
