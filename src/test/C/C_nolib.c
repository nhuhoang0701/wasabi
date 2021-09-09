#include <test_tools/TestAssert.h>

//TODO: Centralyze
#define EXPORT

EXPORT static char staticstr[] = "THIS IS A STATIC CHAR*";


/*********************************************
* Basics numerical operator  test
*/
EXPORT int i_MULT_i(int a)
{
	return a*a;
}
EXPORT int i_DIV_i(int a)
{
	return a/a;
}
EXPORT int i_ADD_i(int a)
{
	return a+a;
}
EXPORT int i_SUB_i(int a)
{
	return a+a;
}

EXPORT int i_DIV_i_i(int a, int b)
{
	return a/b;
}

EXPORT double d_DIV_d_d(double a, double b)
{
	return a/b;
}


/*********************************************
* Basics C char tests
*/
EXPORT const char* str_STATICSTR_c(char firstChar)
{
	staticstr[0] = firstChar;
	return staticstr;
}

EXPORT const char* str_LOCALSTR_c(char firstChar)
{
	char localstr[] = "THIS IS A LOCAL CHAR*";
	localstr[0] = firstChar;
	return localstr;
}

EXPORT char c_ITOCHAR_i(unsigned int i)
{
	if(i > 9)
		return '#';

	return i + '0';
}

EXPORT char* str_TOLOWER_str(char* a)
{
	if( a )
	{
		char *b = a;
		while ( *b )
		{
			if( *b >= 'A' && *b <= 'Z')
				*b += ('a' - 'A');
			b++;
		}
	}
	return a;
}

EXPORT char* str_TOLOWER_pv(void* a)
{
	return str_TOLOWER_str(a);
}


/*********************************************
* Basics C function pointer tests
*/
typedef int (*ptr_on_i_FCT_i)(int);
EXPORT int i_CALLFCT_pv_i(ptr_on_i_FCT_i ordinal, int val)
{
	ptr_on_i_FCT_i ptr = ordinal;
	return ptr(val);
}

EXPORT int i_CALLFCT_I_DIV_I_i(int i)
{
	ptr_on_i_FCT_i ptr = i_DIV_i;
	return ptr(i);
}

EXPORT int i_CALLFCT_I_MULT_I_i(int i)
{
	ptr_on_i_FCT_i ptr = i_MULT_i;
	return ptr(i);
}

EXPORT int i_CALLFCT_I_ADD_I_i(int i)
{
	ptr_on_i_FCT_i ptr = i_ADD_i;
	return ptr(i);
}

EXPORT ptr_on_i_FCT_i pv_getFctPtr_str(const char* fctName)
{
	if( fctName[12] == 'D')
	{
		return i_CALLFCT_I_DIV_I_i;
	}
	if( fctName[12] == 'M')
	{
		return i_CALLFCT_I_MULT_I_i;
	}
	if( fctName[12] == 'A')
	{
		return i_CALLFCT_I_ADD_I_i;
	}

	return 0;
}



/*********************************************
* TODO: Put the lines after in a separate file
*/
#include <stdio.h>  // For printf
#include <string.h> // For strcmp


void TestNumericalOperator()
{
	C_WASABI_CHECK_EQUAL(i_MULT_i(2), 4)
	C_WASABI_CHECK_EQUAL(i_DIV_i(2), 1);
	C_WASABI_CHECK_EQUAL(i_ADD_i (2),  4);
	C_WASABI_CHECK_EQUAL(i_SUB_i (2), 4);
	C_WASABI_CHECK_EQUAL(i_DIV_i_i(4,4), 1);
	C_WASABI_CHECK_EQUAL(d_DIV_d_d(4,4), 1);
}

void TestChar_C()
{
	CPPUNIT_ASSERT_EQUAL_STR(str_STATICSTR_c('c'), "cHIS IS A STATIC CHAR*");
	// CPPUNIT_ASSERT_EQUAL_STR(str_LOCALSTR_c('c'), "cHIS IS A LOCAL CHAR*");
	C_WASABI_CHECK_EQUAL(c_ITOCHAR_i(8), '8');
	char* pC = "STRING TO LOWER";
	CPPUNIT_ASSERT_EQUAL_STR(str_TOLOWER_str(pC), "string to lower");
	void* pV = (void*)pC;
	CPPUNIT_ASSERT_EQUAL_STR(str_TOLOWER_pv(pV), "string to lower");
}

void Test_CFunctionPointer()
{
	/*
	i_CALLFCT_pv_i
	i_CALLFCT_I_DIV_I_i
	i_CALLFCT_I_MULT_I_i
	i_CALLFCT_I_ADD_I_i
	*/


	//printf("Call i_CALLFCT_pv_i \n");
	//ptr_on_i_FCT_i pFct;
	//int res = i_CALLFCT_pv_i(pFct, 3);

	//printf("val : %d \n\n",res);
	//if (res == -1)
	//if (strcmp( pRes, "string to lower"))
	//	return -1;
}

int main()
{
	TEST_INIT();

	TestNumericalOperator();
	TestChar_C();
	Test_CFunctionPointer();

	return TEST_HAVEERROR();
}
