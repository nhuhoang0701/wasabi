#include "sqlite.h"

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

#include <sqlite3.h>

#include <iostream>

const bool useDBFile = false;
namespace dbproxy
{
	DBSQLite::DBSQLite(const std::string& dbname)
	: DBProxy()
	{
		std::cout << "dbname:" << dbname << std::endl;
		int res = 0;
		if(useDBFile)
		{
			res = sqlite3_open_v2(std::string("./resources/sqlite/"+dbname+"/"+dbname+".db").c_str(), &m_sqlite_db, SQLITE_OPEN_READONLY, nullptr);
		}
		else
			res = sqlite3_open(":memory:", &m_sqlite_db);
		if (res != SQLITE_OK)
		{
			const std::string err = sqlite3_errmsg(m_sqlite_db);
			std::cerr << "Cannot open database: '" << err << std::endl;
			sqlite3_close(m_sqlite_db);
			
			throw std::runtime_error("Cannot open database:" + err);
		}

		if(useDBFile == false)
		{
			//***********************************************************
			// Create a DB on the fly with a SQL , waiting WASI read file
			
			if(dbname == "onetable_datatype.db")
			{
				// Create the table
				executeSQL(createOneTableSQL, nullptr);
				// insert data
				executeSQL(data_onetable_datatype, nullptr);
			}
			else if(dbname == "efashion.db")
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

		//***********************************************************
		// Retrieve the tables from the DB
		std::function<void(const Row&)> lambda = [this](const Row& row)
		{
			TableDescr table;
			table.setName(row[1].getString());
			
			getTables().push_back(table);
		};
		executeSQL("SELECT * FROM sqlite_master WHERE type='table';", &lambda);

		for(auto& table : getTables())
		{
			//std::cout << ">>*******************************************************" << std::endl;
			//std::cout << "Table name : '" << table.getName() << "'" << std::endl;
			std::function<void(const Row&)> lambda = [&table](const Row& row)
			{
				table.push_back(ColumnDescr(row[1].getString(), row[2].getString()));
			};
			executeSQL("PRAGMA table_info("+table.getName()+");", &lambda);
			//std::cout << "<<*******************************************************" << std::endl << std::endl;
		}
		/*
		{
			sqlite3_stmt* stmt = nullptr;
			int rc = sqlite3_prepare_v2(m_sqlite_db, "SELECT * FROM pragma_table_info('sometable');", -1, &stmt, 0);
			printf("first query rc = %d\n", rc);
			while( sqlite3_step(stmt)==SQLITE_ROW )
			{
				for(int i=0; i<sqlite3_column_count(stmt); i++)
				{
					if( i ) 
						printf(",");
					printf("%s", sqlite3_column_text(stmt,i));
				}
				printf("\n");
			}
			sqlite3_finalize(stmt);
		}
		*/
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
			if(argv[i] == nullptr)
				row.push_back(Value("##NULL")); //TODO: Manage NULL Value
			else
				row.push_back(Value(argv[i]));
			//std::cout << "sqlite_callback SQL result: '" << azColName[i] << "' = '" << (argv[i] ? argv[i] : "NULL") << "'" << std::endl;
		}
		(*calback)(row);
		
		return 0;
	}
} // dbproxy
