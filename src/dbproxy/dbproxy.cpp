#include "dbproxy.h"

#include <stdexcept>

DBProxy DBProxy::getDBProxy(const std::string& cnxString)
{
	return DBProxy(cnxString);
}

DBProxy::DBProxy(const std::string& cnxString)
{
	TableDescr table1;
	table1.m_name = "Table1.name";
	table1.m_columns.push_back(ColumnDescr("ColumnName0", "string"));
	table1.m_columns.push_back(ColumnDescr("ColumnName1", "string"));
	table1.m_columns.push_back(ColumnDescr("ColumnName2", "numeric"));
	table1.m_columns.push_back(ColumnDescr("ColumnName3", "numeric"));
	
	m_tablesDescr.push_back(table1);
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

void  DBProxy::executeSQL(const std::string& SQL, std::function<void (Row const&)> calback)const
{
	Row row1;
	row1.push_back(Value("IH"));
	row1.push_back(Value("GH"));
	row1.push_back(Value(84));
	row1.push_back(Value(77));
	
	calback(row1);
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
