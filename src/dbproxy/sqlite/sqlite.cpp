#include "sqlite.h"

#include "efashion/createtables.h"
#include "efashion/insertdata.h"

#include <sqlite3.h>

#include <iostream>


namespace dbproxy
{
	DBSQLite::DBSQLite()
	: DBProxy()
	{
		if (sqlite3_open(":memory:", &m_sqlite_db) != SQLITE_OK)
		//if (sqlite3_open_v2("../sqlite/efashion.db", &m_sqlite_db, SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK)
		{
			const std::string err = sqlite3_errmsg(m_sqlite_db);
			std::cerr << "Cannot open database: '" << err << std::endl;
			sqlite3_close(m_sqlite_db);
			
			throw std::runtime_error("Cannot open database:" + err);
		}

		//***********************************************************
		// Create a DB on the fly with a SQL , waiting WASI read file
		
		// Create the tables
		if (sqlite3_exec(m_sqlite_db, createFashionTablesSQL, 0, 0, &sqlite_err_msg) != SQLITE_OK )
		{
			const std::string err_msg(sqlite_err_msg);
			sqlite3_free(sqlite_err_msg);
			std::cerr << "Failed to create tables, SQL error: '" << err_msg << "'" << std::endl;
			throw std::runtime_error("sqlite3_exec: " + std::string(err_msg));
		}
		else
		{
			std::cout << "Tables created successfully" << std::endl;
		}
		// insert data
		if (sqlite3_exec(m_sqlite_db, eFashionInsertDataInto_table_Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma, 0, 0, &sqlite_err_msg) != SQLITE_OK )
		{
			const std::string err_msg(sqlite_err_msg);
			sqlite3_free(sqlite_err_msg);
			std::cerr << "Failed to insert data, SQL error: '" << err_msg << "'" << std::endl;
			throw std::runtime_error("sqlite3_exec: " + std::string(err_msg));
		}
		else
		{
			std::cout << "Table eFashionInsertDataInto_table_Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma data inserted successfully" << std::endl;
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
			//std::cout << ">>*******************************************************" << std::endl;
			//std::cout << "Table name : '" << table.getName() << "'" << std::endl;
			std::function<void(const Row&)> lambda = [&table](const Row& row)
			{
				table.push_back(ColumnDescr(row[1].getString(), row[2].getString()));
			};
			executeSQL("PRAGMA table_info("+table.getName()+");", lambda);
			//std::cout << "<<*******************************************************" << std::endl << std::endl;
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
		//std::cout << ">>" << __func__ << ": " << SQL << std::endl;
		if (sqlite3_exec(m_sqlite_db, SQL.c_str(), sqlite_callback, reinterpret_cast<void*>(&calback), &sqlite_err_msg) != SQLITE_OK )
		{ 
			std::cerr <<  "Failed to execute SQL, SQL error: '" <<  sqlite_err_msg << "'" << std::endl;
			const std::string err_msd(sqlite_err_msg);
			sqlite3_free(sqlite_err_msg);
			throw std::runtime_error(err_msd);
		}
		else
		{    
			//std::cout << "<< SQL successfully executed" << std::endl;
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
			//std::cout << "sqlite_callback SQL result: '" << azColName[i] << "' = '" << (argv[i] ? argv[i] : "NULL") << "'" << std::endl;
		}
		(*calback)(row);
		
		return 0;
	}
} // dbproxy
