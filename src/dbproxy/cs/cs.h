#pragma once

#include "../dbproxy.h"

class cs;

namespace dbproxy
{
	class DBConnectionSever : public DBProxy
	{
	public:
		DBConnectionSever(const std::string& dbname);
		virtual ~DBConnectionSever();

		virtual void executeSQL(const std::string& SQL, const std::function<void (Row const&)>* calback) const;
	};

} // dbproxy
