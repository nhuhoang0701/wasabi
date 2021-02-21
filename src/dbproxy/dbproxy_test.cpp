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
		
	std::function<void(const Row&)> lambda = [&cols](const Row& row)
	{
		CPPUNIT_ASSERT_EQUAL(row.size(),cols.size());
		CPPUNIT_ASSERT_EQUAL_STR(row[0].m_str.c_str(),"IH");
		CPPUNIT_ASSERT_EQUAL_STR(row[1].m_str.c_str(),"GH");
		CPPUNIT_ASSERT_EQUAL(row[2].m_double,84);
		CPPUNIT_ASSERT_EQUAL(row[3].m_double,77);
		
		std::cout << row[0].m_double << " " << row[1].m_double << " " << row[2].m_str << " " << row[3].m_str << std::endl;
	};
	dbProxy.executeSQL("select * from " + tableNameStr, lambda);
}
