#include "dbproxy.h"

#include <stdexcept>
#include <iostream>

#include <sqlite3.h>

DBProxy DBProxy::getDBProxy(const std::string& cnxString)
{
	return DBProxy(cnxString);
}


sqlite3* sqlite_db = nullptr;
char* sqlite_err_msg = nullptr;
int sqlite_callback(void *, int, char **, char **);

int callback(void* param, int argc, char **argv, char **azColName)
{
    std::function<void (Row const&)>* calback = reinterpret_cast<std::function<void (Row const&)>*>(param);
    
	Row row;
	row.reserve(argc);
    for (int i = 0; i < argc; i++)
	{
		//TODO: argv[i] == nullptr
		row.push_back(Value(argv[i]));
        std::cout << "callback SQL result: '" << azColName[i] << "' = '" << (argv[i] ? argv[i] : "NULL") << "'" << std::endl;
    }
	(*calback)(row);
    
    return 0;
}



DBProxy::DBProxy(const std::string& cnxString)
{
	//***********************************************************
	// Create a DB on the fly
    if (sqlite3_open(":memory:", &sqlite_db) != SQLITE_OK)
	{    
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(sqlite_db));
        sqlite3_close(sqlite_db);
        
        throw std::runtime_error("Cannot open database");
    }
	
	const char *sql =	\
						"CREATE TABLE Table1 (ColumnName0 TEXT, ColumnName1 TEXT, ColumnName2 REAL, ColumnName3 REAL);"
						"INSERT INTO Table1(ColumnName0, ColumnName1, ColumnName2, ColumnName3) VALUES ('FR', 'Paris',12.0, 2.0);"
						"INSERT INTO Table1(ColumnName0, ColumnName1, ColumnName2, ColumnName3) VALUES ('FR', 'Bordeaux',7, 3);"
						"CREATE TABLE Table2 (ColumnName0 TEXT, ColumnName1 TEXT);"
						"INSERT INTO Table2(ColumnName0, ColumnName1) VALUES ('FR', 'Paris');"
						"INSERT INTO Table2(ColumnName0, ColumnName1) VALUES ('FR', 'Bordeaux');";
    if (sqlite3_exec(sqlite_db, sql, 0, 0, &sqlite_err_msg) != SQLITE_OK )
	{
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", sqlite_err_msg);
        //TODO: sqlite3_free(sqlite_err_msg);
        throw std::runtime_error(sqlite_err_msg);        
    }
	else
	{
        std::cout << "Table Table1 created successfully" << std::endl;
    }

	//***********************************************************
	// Retrieve the tables from the DB
	std::function<void(const Row&)> lambda = [this](const Row& row)
	{
		TableDescr table;
		table.m_name = row[1].getString();
		
		m_tablesDescr.push_back(table);
	};
	executeSQL("SELECT * FROM sqlite_master WHERE type='table';", lambda);
	
	for(auto& table : m_tablesDescr)
	{
		std::function<void(const Row&)> lambda = [&table](const Row& row)
		{
			table.m_columns.push_back(ColumnDescr(row[1].getString(), row[2].getString()));
		};
		executeSQL("PRAGMA table_info("+table.getName()+");", lambda);
	}
}

DBProxy::~DBProxy()
{
    sqlite3_close(sqlite_db);
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
	std::cout << __func__ << ":" << SQL << std::endl;
    if (sqlite3_exec(sqlite_db, SQL.c_str(), callback, reinterpret_cast<void*>(&calback), &sqlite_err_msg) != SQLITE_OK )
	{    
        fprintf(stderr, "Failed to execute SQL\n");
        fprintf(stderr, "SQL error: %s\n", sqlite_err_msg);
        //TODO: sqlite3_free(sqlite_err_msg);
        throw std::runtime_error(sqlite_err_msg);
    }
	else
	{    
        std::cout << "SQL successfully executed" << std::endl;
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
