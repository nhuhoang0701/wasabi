#pragma once
#include <stdio.h>

static bool wasabi_cppunit_haveerror = 0;
#define TEST_INIT() {wasabi_cppunit_haveerror = 0;}
#define TEST_HAVEERROR() (wasabi_cppunit_haveerror)
#define TEST_SETERROR() { wasabi_cppunit_haveerror = 1;}


#define WASABI_xstr(s) WASABI_str(s)

#define WASABI_str(s) #s

#define ASSERT_MESSAGE(m) (printf("FAILED assertion: %s From function: %s at %s: %d  \n", m, __func__, __FILE__, __LINE__))

#define CPPUNIT_ASSERT_MSG(x, msg) \
{\
	if (!(x))\
	{\
		ASSERT_MESSAGE(msg);\
		TEST_SETERROR();\
	}\
	else\
	{\
		printf("OK: %s  \n", WASABI_xstr(x));\
	}\
}

#define CPPUNIT_ASSERT(x) \
{\
	CPPUNIT_ASSERT_MSG(x, WASABI_xstr(x));\
}

#define CPPUNIT_ASSERT_IGNORED(x) \
{\
	{\
		printf("IGNORED: %s \n", WASABI_xstr(x));\
	}\
}

#define CPPUNIT_ASSERT_EQUAL(x,y) CPPUNIT_ASSERT((x) == (y) );
#define CPPUNIT_ASSERT_NOTEQUAL(x,y) CPPUNIT_ASSERT((x) != (y) );

#define CPPUNIT_ASSERT_EQUAL_STR(x,y) CPPUNIT_ASSERT_MSG((strcmp(x, y) == 0), x );

#define LDE_CPPUNIT_EXCEPTION(x, y)



// Not htread safe!!!!!

// Usage:
// int test_func()
// {
//   TEST_INIT();
//
//   //... the tests
//	 CPPUNIT_ASSERT_EQUAL(.....);	
//
//   eturn TEST_HAVEERROR();
// }
//
