#include "sqlite.h"

#include "dbproxy/dbproxy.h"

#if defined(DB_FILE_EMBDED)
#include "onetable_datatype/createtable.h"
#include "onetable_datatype/table_onetable_datatype.h"

#include "efashion/createtables.h"
#include "efashion/table_Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma.h"
#include "efashion/table_Agg_yr_qt_rn_st_ln_ca_sr.h"
#include "efashion/table_Article_Color_Lookup.h"
#include "efashion/table_Article_lookup.h"
#include "efashion/table_Article_Lookup_Criteria.h"
#include "efashion/table_Calendar_year_lookup.h"
#include "efashion/table_Outlet_Lookup.h"
#include "efashion/table_product_promotion_facts.h"
#include "efashion/table_promotion_lookup.h"
#include "efashion/table_Shop_facts.h"
#endif

#include <iterator>
#include <sqlite3.h>

#include <iostream>

namespace dbproxy
{
#if !defined (DB_FILE_EMBDED)
	// Workaround as PRAGMA table_info didn't works
	static sqlite3* g_db = nullptr;
	static TableDescr* g_tableDescr = nullptr;
	// Workaround as PRAGMA table_info didn't works
#endif

	DBSQLite::DBSQLite(const std::string& dbname)
	: DBProxy()
	{
		std::cout << "dbname: '" << dbname << "'" << std::endl;
		int res = 0;
#if defined(DB_FILE_EMBDED)
		res = sqlite3_open(":memory:", &m_sqlite_db);
#else
		std::string file("./resources/sqlite/"+dbname+"/"+dbname+".db");
		std::cout << "db  file: '" << file << "'" << std::endl;
		res = sqlite3_open_v2(file.c_str(), &m_sqlite_db, SQLITE_OPEN_READONLY, nullptr);
#endif

		if (res != SQLITE_OK)
		{
			const std::string err = sqlite3_errmsg(m_sqlite_db);
			std::cerr << "Cannot open database: '" << err << std::endl;
			sqlite3_close(m_sqlite_db);
			
			throw std::runtime_error("Cannot open database:" + err);
		}

#if defined(DB_FILE_EMBDED)
		{
			//***********************************************************
			// Create a DB on the fly with a SQL , waiting WASI read file
			
			if(dbname == "onetable_datatype")
			{
				// Create the table
				executeSQL(createOneTableSQL, nullptr);
				// insert data
				executeSQL(data_onetable_datatype, nullptr);
			}
			else if(dbname == "efashion")
			{
				// Create the tables
				executeSQL(createFashionTablesSQL, nullptr);
				// insert data				// insert data
				executeSQL(data_Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma, nullptr);
				executeSQL(data_Agg_yr_qt_rn_st_ln_ca_sr, nullptr);
				executeSQL(data_Article_Color_Lookup, nullptr);
				executeSQL(data_Article_lookup, nullptr);
				executeSQL(data_Article_Lookup_Criteria, nullptr);
				executeSQL(data_Calendar_year_lookup, nullptr);
				executeSQL(data_Outlet_Lookup, nullptr);
				executeSQL(data_product_promotion_facts, nullptr);
				executeSQL(data_promotion_lookup, nullptr);
				executeSQL(data_Shop_facts, nullptr);
			}
			else
			{
				throw std::runtime_error("Unknow dbname: '"  + dbname + "'");
			}
		}
#endif //DB_FILE_EMBDED

		// ***********************************************************
		// Retrieve the tables from the DB
		std::function<void(const Row&)> lambda = [this](const Row& row)
		{
			TableDescr table;
			table.setName(row[1].getString());
			
			getTables().push_back(table);
		};
		executeSQL("SELECT * FROM sqlite_master WHERE type='table';", &lambda);

		//***********************************************************
		// Retrieve the from the tables
#if !defined (DB_FILE_EMBDED)
		g_db = m_sqlite_db;
		for(auto& table : getTables())
		{
			g_tableDescr = &table;
			executeSQL("SELECT * FROM "+table.getName()+" LIMIT  1;", nullptr);
			g_tableDescr = nullptr;
		}
		g_db = nullptr;
#else // DB_FILE_EMBDED
		for(auto& table : getTables())
		{
			std::function<void(const Row&)> lambda = [&table](const Row& row)
			{
				table.push_back(ColumnDescr(row[1].getString(), row[2].getString()));
			};
			executeSQL("PRAGMA table_info("+table.getName()+");", &lambda);
		}
#endif // DB_FILE_EMBDED
	}

	DBSQLite::~DBSQLite()
	{
		sqlite3_close(m_sqlite_db);
		m_sqlite_db = nullptr;
		sqlite_err_msg = nullptr;
	}

	void DBSQLite::executeSQL(const std::string& SQL, const std::function<void (Row const&)>* calback) const
	{
		//std::cout << ">>" << __func__ << ": " << SQL << std::endl;
		if (sqlite3_exec(m_sqlite_db, SQL.c_str(), sqlite_callback, const_cast<void*>(reinterpret_cast<const void*>(calback)), &sqlite_err_msg) != SQLITE_OK )
		{ 
			std::cerr <<  "Failed to execute SQL '" << SQL << "', SQL error: '" <<  sqlite_err_msg << "'" << std::endl;
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
#if !defined (DB_FILE_EMBDED)
			if(g_db && g_tableDescr)
			{
				char const *pzDataType = nullptr;
				char const *pzCollSeq = nullptr;
				int *pNotNull = nullptr;
				int *pPrimaryKey = nullptr;
				int *pAutoinc = nullptr;
				if(SQLITE_OK ==sqlite3_table_column_metadata(g_db, nullptr, g_tableDescr->getName().c_str(), azColName[i], &pzDataType, &pzCollSeq, pNotNull, pPrimaryKey, pAutoinc) )
				{
					g_tableDescr->push_back(ColumnDescr(std::string(azColName[i]), std::string(pzDataType?pzDataType:"")));
					std::cout <<  g_tableDescr->getName()<< ": '" << azColName[i] << "' = '" << (pzDataType ? pzDataType : "NULL") << "'" << std::endl;
				}
				else
					std::cerr << "ERROR :sqlite3_table_column_metadata" << std::endl;
			}
#endif

			if(calback != nullptr)
			{
				if(argv[i] == nullptr)
					row.push_back(Value("##NULL")); //TODO: Manage NULL Value
				else
					row.push_back(Value(argv[i]));
			}
		}
		if(calback != nullptr)
			(*calback)(row);
		
		return 0;
	}
} // dbproxy
