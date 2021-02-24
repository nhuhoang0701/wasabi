#include "dbproxy.h"

#include "test_tools/TestAssert.h"

int main()
{
	TEST_INIT();

	// The string parameter is just as example
	static const std::string cnxStr("local:sqlite:efashion.db");
	static const std::string tableNameStr("Table1");
	
    std::shared_ptr<DBProxy> dbProxy = DBProxy::getDBProxy(cnxStr);
	
	const std::vector<TableDescr>& tables = dbProxy->getTables();
	
	CPPUNIT_ASSERT_EQUAL(tables.size(),3);
	for(const auto& tableDescr : tables)
	{
		const std::string& tableName = tableDescr.getName();		
		CPPUNIT_ASSERT_EQUAL(tableDescr,dbProxy->getTableDescr(tableName));
	}
	
	
	const TableDescr& tableDescr = dbProxy->getTableDescr(tableNameStr);
	const std::vector<ColumnDescr>& cols = tableDescr.getColumnsDescr();
	CPPUNIT_ASSERT_EQUAL(cols.size(),4);
	CPPUNIT_ASSERT_EQUAL(cols[0].getName(),"ColumnName0");
	CPPUNIT_ASSERT_EQUAL(cols[0].getDataType(),"TEXT");
	CPPUNIT_ASSERT_EQUAL(cols[1].getName(),"ColumnName1");
	CPPUNIT_ASSERT_EQUAL(cols[1].getDataType(),"TEXT");
	CPPUNIT_ASSERT_EQUAL(cols[2].getName(),"ColumnName2");
	CPPUNIT_ASSERT_EQUAL(cols[2].getDataType(),"REAL");
	CPPUNIT_ASSERT_EQUAL(cols[3].getName(),"ColumnName3");
	CPPUNIT_ASSERT_EQUAL(cols[3].getDataType(),"REAL");
	
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&cols, &line](const Row& row)
		{
			if(line == 0)
			{
				CPPUNIT_ASSERT_EQUAL(row.size(),cols.size());
				CPPUNIT_ASSERT_EQUAL_STR(row[0].getString().c_str(),"FR");
				CPPUNIT_ASSERT_EQUAL_STR(row[1].getString().c_str(),"Paris");
				CPPUNIT_ASSERT_EQUAL(std::stod(row[2].getString()),12);
				CPPUNIT_ASSERT_EQUAL(std::stod(row[3].getString()),2000000);
			}
			else if(line == 1)
			{
				CPPUNIT_ASSERT_EQUAL(row.size(),cols.size());
				CPPUNIT_ASSERT_EQUAL_STR(row[0].getString().c_str(),"FR");
				CPPUNIT_ASSERT_EQUAL_STR(row[1].getString().c_str(),"Bordeaux");
				CPPUNIT_ASSERT_EQUAL(std::stod(row[2].getString()),7);
				CPPUNIT_ASSERT_EQUAL(std::stod(row[3].getString()),3000000);
			}
			
			line++;
		};
		dbProxy->executeSQL("SELECT * FROM " + tableNameStr, lambda);
		CPPUNIT_ASSERT_EQUAL(line,2);
	}
	
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&cols, &line](const Row& row)
		{
			CPPUNIT_ASSERT_EQUAL(row.size(), 2);
			CPPUNIT_ASSERT_EQUAL_STR(row[0].getString().c_str(),"FR");
			CPPUNIT_ASSERT_EQUAL(std::stod(row[1].getString()),1666666.66666667);
			line++;
		};
		dbProxy->executeSQL("SELECT ColumnName0,sum(ColumnName3)/3 FROM " + tableNameStr, lambda);
		CPPUNIT_ASSERT_EQUAL(line,1);
	}
	return TEST_HAVEERROR();
}
