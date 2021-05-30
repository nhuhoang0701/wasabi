#pragma once
#include <stdio.h>    // For printf
#include <string.h>   // For strcmp

static int wasabi_cppunit_haveerror = 0;
#define TEST_INIT() {wasabi_cppunit_haveerror = 0;}
#define TEST_HAVEERROR() (wasabi_cppunit_haveerror)
#define TEST_SETERROR() { wasabi_cppunit_haveerror = 1;}


#define WASABI_xstr(s) WASABI_str(s)

#define WASABI_str(s) #s

#define ASSERT_MESSAGE(m) (printf("FAILED assertion: %s From function: %s at %s: %d  \n", m, __func__, __FILE__, __LINE__))

#define CPPUNIT_ASSERT_MSG(assertion, msg) \
{\
	if (!(assertion))\
	{\
		ASSERT_MESSAGE(msg);\
		TEST_SETERROR();\
	}\
	else\
	{\
		printf("OK: %s  \n", WASABI_xstr(assertion));\
	}\
}


#define CPPUNIT_ASSERT(assertion) \
{\
	CPPUNIT_ASSERT_MSG(assertion, WASABI_xstr(assertion));\
}

#if defined (__cplusplus)
#include <iostream>
#define CPPUNIT_ASSERT_LOG(assertion, lhs, rhs) \
{\
	if (!(assertion))\
	{\
		std::cout << "FAILED assertion: '" << WASABI_xstr(assertion) << " ('" << lhs << "','" << rhs << "')  From function: " << __func__ << " at " << __FILE__ << ": " << __LINE__ << "\n";\
		TEST_SETERROR();\
	}\
	else\
	{\
		printf("OK: %s  \n", WASABI_xstr(assertion));\
	}\
}
#endif

#define CPPUNIT_ASSERT_IGNORED(assertion) \
{\
	{\
		printf("IGNORED: %s \n", WASABI_xstr(assertion));\
	}\
}

#define WASABI_CHECK_EQUAL(lhs,rhs) CPPUNIT_ASSERT_LOG((lhs) == (rhs), lhs, rhs);
#define C_WASABI_CHECK_EQUAL(lhs,rhs) CPPUNIT_ASSERT((lhs) == (rhs));

#define WASABI_CHECK_NOTEQUAL(lhs,rhs) CPPUNIT_ASSERT_LOG((lhs) != (rhs), lhs, rhs);
#define C_WASABI_CHECK_NOTEQUAL(lhs,rhs) CPPUNIT_ASSERT((lhs) != (rhs));

#define CPPUNIT_ASSERT_EQUAL_STR(lhs,rhs) CPPUNIT_ASSERT_MSG((strcmp(lhs, rhs) == 0), lhs );

#define LDE_CPPUNIT_EXCEPTION(lhs, rhs)



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
