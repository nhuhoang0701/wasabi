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
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&line](const Row& row)
		{
			if(line == 0)
			{
				WASABI_CHECK_EQUAL(row[0].getStringView(),std::string_view("text1"));
				CPPUNIT_ASSERT_EQUAL_STR(row[2].getString().c_str(),"1.0");
			}
			else if(line == 3)
			{
				CPPUNIT_ASSERT_EQUAL_STR(row[0].getString().c_str(),"text3");
				CPPUNIT_ASSERT_EQUAL_STR(row[2].getString().c_str(),"3.0");
			}
			
			line++;
		};
		dbProxy->executeSQL("SELECT * FROM " + tableNameStr, &lambda);
		//std::cout << "line:" << line << std::endl;
		WASABI_CHECK_EQUAL(line,4);
	}
	
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&line](const Row& row)
		{
			WASABI_CHECK_EQUAL(row.size(), 1);
			CPPUNIT_ASSERT_EQUAL_STR("6.0", row[0].getString().c_str());
			line++;
		};
		dbProxy->executeSQL("SELECT sum(real) FROM " + tableNameStr, &lambda);
		//std::cout << "line:" << line << std::endl;
		WASABI_CHECK_EQUAL(line,1);
	}
	
	{
		size_t line = 0;
		std::function<void(const Row&)> lambda = [&line](const Row& row)
		{
			line++;
		};
		dbProxy->executeSQL("", &lambda);
		//std::cout << "line:" << line << std::endl;
		WASABI_CHECK_EQUAL(line,0);
	}
	return TEST_HAVEERROR();
}
