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
	
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&cols, &line](const Row& row)
		{
			if(line == 0)
			{
				CPPUNIT_ASSERT_EQUAL(row.size(),cols.size());
				std::cout << std::endl;
				CPPUNIT_ASSERT_EQUAL_STR(row[0].getString().c_str(),"1");
				CPPUNIT_ASSERT_EQUAL_STR(row[1].getString().c_str(),"2014");
				CPPUNIT_ASSERT_EQUAL_STR(row[2].getString().c_str(),"3");
				CPPUNIT_ASSERT_EQUAL_STR(row[10].getString().c_str(),"6937.6");
			}
			else if(line == 1977)
			{
				CPPUNIT_ASSERT_EQUAL(row.size(),cols.size());
				CPPUNIT_ASSERT_EQUAL_STR(row[0].getString().c_str(),"1978");
				CPPUNIT_ASSERT_EQUAL_STR(row[1].getString().c_str(),"2017");
				CPPUNIT_ASSERT_EQUAL_STR(row[2].getString().c_str(),"3");
				CPPUNIT_ASSERT_EQUAL_STR(row[10].getString().c_str(),"7618.2");
			}
			
			line++;
		};
		dbProxy->executeSQL("SELECT * FROM " + tableNameStr, &lambda);
		std::cout << "line:" << line << std::endl;
		CPPUNIT_ASSERT_EQUAL(line,3000);
	}
	
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&cols, &line](const Row& row)
		{
			CPPUNIT_ASSERT_EQUAL(row.size(), 2);
			CPPUNIT_ASSERT_EQUAL_STR(row[0].getString().c_str(),"2014");
			CPPUNIT_ASSERT_EQUAL(std::stod(row[1].getString()),12129067.6);
			line++;
		};
		dbProxy->executeSQL("SELECT Yr,sum(Sales_revenue)/3 FROM " + tableNameStr, &lambda);
		std::cout << "line:" << line << std::endl;
		CPPUNIT_ASSERT_EQUAL(line,1);
	}
	return TEST_HAVEERROR();
}
