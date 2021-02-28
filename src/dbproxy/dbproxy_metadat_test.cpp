#include "dbproxy.h"

#include "test_tools/TestAssert.h"

#include <iostream>


int main()
{
	TEST_INIT();

	// The string parameter is just as example
	static const std::string cnxStr("local:sqlite:efashion.db");
	static const std::string tableNameStr("Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma");
	
	using namespace dbproxy;
    std::shared_ptr<DBProxy> dbProxy = DBProxy::getDBProxy(cnxStr);
	
	const std::vector<TableDescr>& tables = dbProxy->getTables();
	CPPUNIT_ASSERT_EQUAL(tables.size(),10);
	for(const auto& tableDescr : tables)
	{
		const std::string& tableName = tableDescr.getName();		
		CPPUNIT_ASSERT_EQUAL(tableDescr,dbProxy->getTableDescr(tableName));
	}
	
	const TableDescr& tableDescr = dbProxy->getTableDescr(tableNameStr);
	const std::vector<ColumnDescr>& cols = tableDescr.getColumnsDescr();
	CPPUNIT_ASSERT_EQUAL(cols.size(),11);
	CPPUNIT_ASSERT_EQUAL(cols[0].getName(),"agg1_id");
	CPPUNIT_ASSERT_EQUAL(cols[0].getDataType(),"INT");
	CPPUNIT_ASSERT_EQUAL(cols[1].getName(),"Yr");
	CPPUNIT_ASSERT_EQUAL(cols[1].getDataType(),"NVARCHAR (4)");
	CPPUNIT_ASSERT_EQUAL(cols[2].getName(),"Qtr");
	CPPUNIT_ASSERT_EQUAL(cols[2].getDataType(),"NVARCHAR (2)");
	CPPUNIT_ASSERT_EQUAL(cols[3].getName(),"Mth");
	CPPUNIT_ASSERT_EQUAL(cols[3].getDataType(),"INT");
	
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
