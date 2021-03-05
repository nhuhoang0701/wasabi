#include "dbproxy.h"

#include "test_tools/TestAssert.h"

#include <iostream>

int main()
{
	TEST_INIT();

	// The string parameter is just as example
	static const std::string cnxStr("local:sqlite:onetable_datatype.db");
	static const std::string tableNameStr("onetable_datatype");
	
	using namespace dbproxy;
    std::shared_ptr<DBProxy> dbProxy = DBProxy::getDBProxy(cnxStr);	
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&line](const Row& row)
		{
			if(line == 0)
			{
				CPPUNIT_ASSERT_EQUAL_STR(row[0].getString().c_str(),"text1");
				CPPUNIT_ASSERT_EQUAL_STR(row[1].getString().c_str(),"1.0");
			}
			else if(line == 2)
			{
				CPPUNIT_ASSERT_EQUAL_STR(row[0].getString().c_str(),"text3");
				CPPUNIT_ASSERT_EQUAL_STR(row[1].getString().c_str(),"3.0");
			}
			
			line++;
		};
		dbProxy->executeSQL("SELECT * FROM " + tableNameStr, &lambda);
		//std::cout << "line:" << line << std::endl;
		CPPUNIT_ASSERT_EQUAL(line,3);
	}
	
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&line](const Row& row)
		{
			CPPUNIT_ASSERT_EQUAL(row.size(), 1);
			CPPUNIT_ASSERT_EQUAL(std::stod(row[1].getString()),3.0);
			line++;
		};
		dbProxy->executeSQL("SELECT sum(real) FROM " + tableNameStr, &lambda);
		//std::cout << "line:" << line << std::endl;
		CPPUNIT_ASSERT_EQUAL(line,1);
	}
	return TEST_HAVEERROR();
}
