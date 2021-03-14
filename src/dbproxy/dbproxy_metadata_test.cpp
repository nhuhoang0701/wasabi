#include "dbproxy.h"

#include <test_tools/TestAssert.h>

#include <iostream>


int main()
{
	TEST_INIT();

	// The string parameter is just as example
	static const std::string cnxStr("local:sqlite:onetable_datatype.db");
	static const std::string tableNameStr("onetable_datatype");
	
	using namespace dbproxy;
    std::shared_ptr<DBProxy> dbProxy = DBProxy::getDBProxy(cnxStr);
	
	const std::vector<TableDescr>& tables = dbProxy->getTables();
	CPPUNIT_ASSERT_EQUAL(tables.size(),1);
	for(const auto& tableDescr : tables)
	{
		const std::string& tableName = tableDescr.getName();		
		CPPUNIT_ASSERT_EQUAL(tableDescr,dbProxy->getTableDescr(tableName));
	}
	
	const TableDescr& tableDescr = dbProxy->getTableDescr(tableNameStr);
	const std::vector<ColumnDescr>& cols = tableDescr.getColumnsDescr();
	CPPUNIT_ASSERT_EQUAL(cols.size(),4);
	CPPUNIT_ASSERT_EQUAL(cols[0].getName(),"text");
	CPPUNIT_ASSERT_EQUAL(cols[0].getDataType(),"TEXT");
	CPPUNIT_ASSERT_EQUAL(cols[1].getName(),"varchar");
	CPPUNIT_ASSERT_EQUAL(cols[1].getDataType(),"VARCHAR");
	CPPUNIT_ASSERT_EQUAL(cols[2].getName(),"real");
	CPPUNIT_ASSERT_EQUAL(cols[2].getDataType(),"REAL");
	CPPUNIT_ASSERT_EQUAL(cols[3].getName(),"integer");
	CPPUNIT_ASSERT_EQUAL(cols[3].getDataType(),"INTEGER");
	
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
