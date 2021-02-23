#pragma once

#include "../dbproxy.h"

class sqlite3;

class DBSQLite : public DBProxy
{
public:
	DBSQLite();
	virtual ~DBSQLite();

	virtual void  executeSQL(const std::string& SQL, std::function<void (Row const&)> calback) const;


private:
	static int sqlite_callback(void* param, int argc, char **argv, char **azColName);

private:
	sqlite3*      m_sqlite_db = nullptr;
	mutable char* sqlite_err_msg = nullptr;
};
