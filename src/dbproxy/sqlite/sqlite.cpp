#include "sqlite.h"

#include "queries.h"

#include <sqlite3.h>

#include <iostream>

DBSQLite::DBSQLite()
: DBProxy()
{
    if (sqlite3_open(":memory:", &m_sqlite_db) != SQLITE_OK)
	{    
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(m_sqlite_db));
        sqlite3_close(m_sqlite_db);
        
        throw std::runtime_error("Cannot open database");
    }

	//***********************************************************
	// Create a DB on the fly with a SQL , waiting WASI read file
    if (sqlite3_exec(m_sqlite_db, createTableSQL, 0, 0, &sqlite_err_msg) != SQLITE_OK )
	{
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", sqlite_err_msg);
        //TODO: sqlite3_free(sqlite_err_msg);
        throw std::runtime_error("sqlite3_exec: " + std::string(sqlite_err_msg));
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
		table.setName(row[1].getString());
		
		getTables().push_back(table);
	};
	executeSQL("SELECT * FROM sqlite_master WHERE type='table';", lambda);
	
	for(auto& table : getTables())
	{
		std::function<void(const Row&)> lambda = [&table](const Row& row)
		{
			table.push_back(ColumnDescr(row[1].getString(), row[2].getString()));
		};
		executeSQL("PRAGMA table_info("+table.getName()+");", lambda);
	}
}

DBSQLite::~DBSQLite()
{
    sqlite3_close(m_sqlite_db);
	m_sqlite_db = nullptr;
	sqlite_err_msg = nullptr;
}

void DBSQLite::executeSQL(const std::string& SQL, std::function<void (Row const&)> calback) const
{
	std::cout << __func__ << ":" << SQL << std::endl;
    if (sqlite3_exec(m_sqlite_db, SQL.c_str(), sqlite_callback, reinterpret_cast<void*>(&calback), &sqlite_err_msg) != SQLITE_OK )
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

int DBSQLite::sqlite_callback(void* param, int argc, char **argv, char **azColName)
{
    std::function<void (Row const&)>* calback = reinterpret_cast<std::function<void (Row const&)>*>(param);
    
	Row row;
	row.reserve(argc);
    for (int i = 0; i < argc; i++)
	{
		//TODO: argv[i] == nullptr
		row.push_back(Value(argv[i]));
        std::cout << "sqlite_callback SQL result: '" << azColName[i] << "' = '" << (argv[i] ? argv[i] : "NULL") << "'" << std::endl;
    }
	(*calback)(row);
    
    return 0;
}
