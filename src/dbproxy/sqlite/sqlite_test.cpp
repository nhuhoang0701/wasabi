#include <sqlite3.h>

#include <test_tools/TestAssert.h>

#include <iostream>

int main()
{
	TEST_INIT();
	
	sqlite3* sqlite_db = nullptr;
	int res = sqlite3_open_v2("./resources/sqlite/efashion/efashion.db", &sqlite_db, SQLITE_OPEN_READONLY, nullptr);
	CPPUNIT_ASSERT_MSG(0==res,"sqlite3_open_v2 return an error.");
	return TEST_HAVEERROR();
}
