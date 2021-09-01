#include "cs.h"

#include "dbproxy/dbproxy.h"

namespace dbproxy
{
	DBConnectionServer::DBConnectionServer(const std::string& dbname)
	: DBProxy()
	{
		
	}

	DBConnectionServer::~DBConnectionServer()
	{

	}

	void DBSQLite::executeSQL(const std::string& SQL, const std::function<void (Row const&)>* calback) const
	{

	}
} // dbproxy
