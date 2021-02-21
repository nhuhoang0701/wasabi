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
	if(SQL.find('*') != std::string::npos)
	{
		{
			Row row;
			row.push_back(Value("FR"));
			row.push_back(Value("Paris"));
			row.push_back(Value(12));
			row.push_back(Value(2));
			
			calback(row);
		}
		{
			Row row;
			row.push_back(Value("FR"));
			row.push_back(Value("Bordeaux"));
			row.push_back(Value(7));
			row.push_back(Value(3));
			
			calback(row);
		}
	}
	else
	{
		{
			Row row;
			row.push_back(Value("FR"));
			row.push_back(Value(5));
			
			calback(row);
		}
	}
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
