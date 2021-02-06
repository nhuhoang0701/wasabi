#pragma once

#include <string>
#include <iostream>


#define WASABI_xstr(s) WASABI_str(s)

#define WASABI_str(s) #s

#define ASSERT_MESSAGE(m)					  \
	(std::cout << "FAILED assertion: " << (m) \
	 << " From function: " << __func__ \
	 << " at " << __FILE__ << ":" << __LINE__ << std::endl)

#define CPPUNIT_ASSERT_MSG(x, msg) \
{\
	if (!(x))\
	{\
		ASSERT_MESSAGE(msg);\
		if (res==0)\
		{\
			res=-1;\
		}\
	}\
	else\
	{\
		std::cout << "OK:" << WASABI_xstr(x) << std::endl;\
	}\
}

#define CPPUNIT_ASSERT(x) \
{\
	CPPUNIT_ASSERT_MSG(x, WASABI_xstr(x));\
}

#define CPPUNIT_ASSERT_IGNORED(x) \
{\
	{\
		std::cout << "IGNORED:" << WASABI_xstr(x) << std::endl;\
	}\
}

#define CPPUNIT_ASSERT_EQUAL(x,y) CPPUNIT_ASSERT((x) == (y) );

#define CPPUNIT_ASSERT_EQUAL_STR(x,y) CPPUNIT_ASSERT_MSG((strcmp(x, y) == 0), x );

#define LDE_CPPUNIT_EXCEPTION(x, y)





// Usage:
// int test_func()
// {
//   int res = 0;
//   TestResultPrinter tr("test_func", res)
//   //... the tests
//   return res;
// }
//
class TestScopePrinter
{
public:
	TestScopePrinter(const std::string& testName, int& res)
		: m_name(testName), m_testStatus(res)
	{
		std::cout << "Testing " << m_name << "..." << std::endl;
	}

	~TestScopePrinter()
	{
		if (m_testStatus == 0)
			std::cout << "Test ok: " << m_name << std::endl;
		else
			std::cout << "Test FAILED: " << m_name << std::endl;
	}

private:
	std::string m_name;
	int& m_testStatus;
};
