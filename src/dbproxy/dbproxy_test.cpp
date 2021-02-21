#include "dbproxy.h"

#include "test_tools/TestAssert.h"

int main() {
	static const std::string cnxStr("local:sqlite:efashion.db");
	static const std::string tableNameStr("Table1.name");
	
	// The string parameter is just as example
    DBProxy dbProxy = DBProxy::getDBProxy(cnxStr);
	
	const std::vector<TableDescr>& tables = dbProxy.getTables();
	
	CPPUNIT_ASSERT_EQUAL(tables.size(),1);
	for(const auto& tableDescr : tables)
	{
		const std::string& tableName = tableDescr.getName();		
		CPPUNIT_ASSERT_EQUAL(tableDescr,dbProxy.getTableDescr(tableName));
	}
	
	
	const TableDescr& tableDescr = dbProxy.getTableDescr(tableNameStr);
	const std::vector<ColumnDescr>& cols = tableDescr.getColumnsDescr();
	CPPUNIT_ASSERT_EQUAL(cols.size(),4);
	CPPUNIT_ASSERT_EQUAL(cols[0].getName(),"ColumnName0");
	CPPUNIT_ASSERT_EQUAL(cols[0].getDataType(),"string");
	CPPUNIT_ASSERT_EQUAL(cols[1].getName(),"ColumnName1");
	CPPUNIT_ASSERT_EQUAL(cols[1].getDataType(),"string");
	CPPUNIT_ASSERT_EQUAL(cols[2].getName(),"ColumnName2");
	CPPUNIT_ASSERT_EQUAL(cols[2].getDataType(),"numeric");
	CPPUNIT_ASSERT_EQUAL(cols[3].getName(),"ColumnName3");
	CPPUNIT_ASSERT_EQUAL(cols[3].getDataType(),"numeric");
	
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&cols, &line](const Row& row)
		{
			if(line == 0)
			{
				CPPUNIT_ASSERT_EQUAL(row.size(),cols.size());
				CPPUNIT_ASSERT_EQUAL_STR(row[0].m_str.c_str(),"FR");
				CPPUNIT_ASSERT_EQUAL_STR(row[1].m_str.c_str(),"Paris");
				CPPUNIT_ASSERT_EQUAL(row[2].m_double,12);
				CPPUNIT_ASSERT_EQUAL(row[3].m_double,2);
			}
			else if(line == 1)
			{
				CPPUNIT_ASSERT_EQUAL(row.size(),cols.size());
				CPPUNIT_ASSERT_EQUAL_STR(row[0].m_str.c_str(),"FR");
				CPPUNIT_ASSERT_EQUAL_STR(row[1].m_str.c_str(),"Bordeaux");
				CPPUNIT_ASSERT_EQUAL(row[2].m_double,7);
				CPPUNIT_ASSERT_EQUAL(row[3].m_double,3);
			}
			
			line++;
		};
		dbProxy.executeSQL("SELECT * FROM " + tableNameStr, lambda);
		CPPUNIT_ASSERT_EQUAL(line,2);
	}
	
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&cols, &line](const Row& row)
		{
			CPPUNIT_ASSERT_EQUAL(row.size(), 2);
			CPPUNIT_ASSERT_EQUAL_STR(row[0].m_str.c_str(),"FR");
			CPPUNIT_ASSERT_EQUAL(row[1].m_double,5);
			line++;
		};
		dbProxy.executeSQL("SELECT ColumnName0,sum(ColumnName3) FROM " + tableNameStr, lambda);
		CPPUNIT_ASSERT_EQUAL(line,1);
	}
}
