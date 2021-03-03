#pragma once

#include "../dbproxy.h"

class sqlite3;



namespace dbproxy
{
	class DBSQLite : public DBProxy
	{
	public:
		DBSQLite(const std::string& dbname);
		virtual ~DBSQLite();

		virtual void  executeSQL(const std::string& SQL, const std::function<void (Row const&)>* calback) const;


	private:
		static int sqlite_callback(void* param, int argc, char **argv, char **azColName);

	private:
		sqlite3*      m_sqlite_db = nullptr;
		mutable char* sqlite_err_msg = nullptr;
	};

} // dbproxy
