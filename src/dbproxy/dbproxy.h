#pragma once

#include <string>
#include <vector>
#include <variant>


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

class Cell
{
public:
	std::string m_cell;
};

class Row
{
	public:
	std::vector<Cell>  m_cells;
};

class DBProxy
{
public:
	static DBProxy getDBProxy(const std::string& cnxString);

	const std::vector<TableDescr>& getTables() const;
	const TableDescr& getTableDescr(const std::string& name) const;

private:
	DBProxy(const std::string& cnxString);

private:
	std::vector<TableDescr>  m_tablesDescr;
};
