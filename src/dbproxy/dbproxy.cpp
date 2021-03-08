#include "dbproxy.h"

#include "sqlite/sqlite.h"

#include <stdexcept>
#include <iostream>
#include <string>
#include <map>
#include <sstream>


namespace dbproxy
{
	std::map<std::string /*cnxString*/, std::shared_ptr<DBProxy>>  proxies;

	std::shared_ptr<DBProxy> DBProxy::getDBProxy(const std::string& cnxString)
	{
		std::string segment;
		std::vector<std::string> seglist;
		std::stringstream cnxStringSS;
		cnxStringSS.str(cnxString);
		while(std::getline(cnxStringSS, segment, ':'))
			seglist.push_back(segment);

		if(seglist[0] == "local")
		{
			if(seglist[1] == "sqlite")
			{
				if(proxies.find(cnxString) == proxies.end())
					proxies[cnxString] = std::make_shared<DBSQLite>(seglist[2]);
				return proxies[cnxString];
			}
			else
				throw std::runtime_error("DBProxy::local db unknow:" + seglist[1]);
		}
		else if(seglist[0] == "remote")
			throw std::runtime_error("DBProxy::remote not implemented");
		else
			throw std::runtime_error("DBProxy::unknow cnx string format/value:" + cnxString);
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
} // dbproxy
