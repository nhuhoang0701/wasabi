#include "dbproxy.h"

#include <test_tools/TestAssert.h>

#include <iostream>


int main()
{
	TEST_INIT();

	// The string parameter is just as example
	static const std::string cnxStr("local:sqlite:onetable_datatype");
	static const std::string tableNameStr("onetable_datatype");
	
	using namespace dbproxy;
    std::shared_ptr<DBProxy> dbProxy = DBProxy::getDBProxy(cnxStr);
	
	const std::vector<TableDescr>& tables = dbProxy->getTables();
	WASABI_CHECK_EQUAL(tables.size(),1);
	for(const auto& tableDescr : tables)
	{
		const std::string& tableName = tableDescr.getName();		
		WASABI_CHECK_EQUAL(&tableDescr, &dbProxy->getTableDescr(tableName));
	}
	
	const TableDescr& tableDescr = dbProxy->getTableDescr(tableNameStr);
	const std::vector<ColumnDescr>& cols = tableDescr.getColumnsDescr();
	WASABI_CHECK_EQUAL(cols.size(),4);
	WASABI_CHECK_EQUAL(cols.at(0).getName(),"text");
	WASABI_CHECK_EQUAL(cols.at(0).getDataType(),"TEXT");
	WASABI_CHECK_EQUAL(cols.at(1).getName(),"varchar");
	WASABI_CHECK_EQUAL(cols.at(1).getDataType(),"VARCHAR");
	WASABI_CHECK_EQUAL(cols.at(2).getName(),"real");
	WASABI_CHECK_EQUAL(cols.at(2).getDataType(),"REAL");
	WASABI_CHECK_EQUAL(cols.at(3).getName(),"integer");
	WASABI_CHECK_EQUAL(cols.at(3).getDataType(),"INTEGER");
	
	/*
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&cols, &line](const Row& row)
		{
			//if(line == 0)
			{
				for(auto c : row)
					std::cout << c << "\t|";
				std::cout << std::endl;
				
			}
			
			line++;
		};
		
		
		
		line = 0;
		dbProxy->executeSQL("SELECT * FROM pragma_table_info('Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma');", &lambda);
		std::cout << "line:" << line << std::endl;
		
		line = 0;
		dbProxy->executeSQL("SELECT * FROM PRAGMA_TABLE_INFO('Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma') WHERE name='sql';", &lambda);
		std::cout << "line:" << line << std::endl;
		
		line = 0;
		dbProxy->executeSQL("PRAGMA table_info('Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma');", &lambda);
		std::cout << "line:" << line << std::endl;		
	}
	*/

	return TEST_HAVEERROR();
}
