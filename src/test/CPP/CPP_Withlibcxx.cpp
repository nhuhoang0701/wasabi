#include <iostream> 
#include "CPP_libcxx.hpp"

#include "test_tools/TestAssert.h"

int main()
{	
	TEST_INIT();

	CPPUNIT_ASSERT_EQUAL(d_STDSQRT_d(4),2);

	CPPUNIT_ASSERT_EQUAL_STR(str_VECTOR_DOUBLE_d(13),"Test OK");
	
	
	int* pRes = NULL;
	pRes = pi_NEW_INT_v();
	CPPUNIT_ASSERT(pRes);

	std::cout << "Call v_DELETE_pv \n";
	v_DELETE_pv(pRes);

	return TEST_HAVEERROR();
}


