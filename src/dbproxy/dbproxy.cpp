#include "dbproxy.h"

#include "sqlite/sqlite.h"

#include <stdexcept>
#include <iostream>

std::shared_ptr<DBProxy> DBProxy::getDBProxy(const std::string& cnxString)
{
	return std::make_shared<DBSQLite>();
}

DBProxy::DBProxy()
{
}

DBProxy::~DBProxy()
{
}

std::vector<TableDescr>& DBProxy::getTables()
{
	return m_tablesDescr;
}

const std::vector<TableDescr>& DBProxy::getTables() const
{
	return m_tablesDescr;
}

const TableDescr& DBProxy::getTableDescr(const std::string& name) const
{
	for(auto& table : m_tablesDescr)
	{
		if(table.getName() == name)
			return table;
	}
	throw std::runtime_error("table didn't exist: '" + name + "'");
}

bool TableDescr::operator == (const TableDescr& rhs) const
{
	if(m_name == rhs.getName() && getColumnsDescr() == rhs.getColumnsDescr())
		return true;
	return false;
}

bool ColumnDescr::operator == (const ColumnDescr& rhs) const
{
	if(m_name == rhs.getName() && getDataType() == rhs.getDataType())
		return true;
	return false;
}
