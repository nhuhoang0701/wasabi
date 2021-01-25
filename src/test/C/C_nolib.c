
#define EXPORT __declspec(dllexport)

EXPORT static char* staticstr = "THIS IS A STATIC CHAR*";

int main()
{
	int res = TestNumericalOperator();
	if (res != 0)
	{
		return -1;
	}

	res = TestChar_C();
	if (res != 0)
	{
		return -1;
	}

	res = Test_CFunctionPointer();
	if (res != 0)
	{
		return -1;
	}

	return 0;
}


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

int TestNumericalOperator()
{
	printf("Call i_MULT_i \n");
	int res = i_MULT_i(2);
	printf("val : %d \n\n",res);
	if (res != 4)
	{
		return -1;
	}
	
	printf("Call i_DIV_i \n");
	res = i_DIV_i(2);
	printf("val : %d \n\n",res);
	if (res != 1)
	{
		return -1;
	}
	
	printf("Call i_ADD_i \n");
	res = i_ADD_i (2);
	printf("val : %d \n\n",res);
	if (res != 4)
	{
		return -1;
	}
	
	
	printf("Call i_SUB_i \n");
	res = i_SUB_i (2);
	printf("val : %d \n\n",res);
	if (res != 4)
	{
		return -1;
	}
	
	
	printf("Call i_DIV_i_i \n");
	res = i_DIV_i_i(4,4);
	printf("val : %d \n\n",res);
	if (res != 1)
	{
		return -1;
	}
	
	
	printf("Call d_DIV_d_d \n");
	res = d_DIV_d_d(4,4);
	printf("val : %d \n\n",res);
	if (res != 1)
	{
		return -1;
	}
	
	return 0;
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
	char* localstr = "THIS IS A LOCAL CHAR*";
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

int TestChar_C()
{
	printf("Call str_STATICSTR_c \n");
	const char* pRes = str_STATICSTR_c('c');
	printf("val : %s \n\n",pRes);
	if (strcmp( pRes, "cHIS IS A STATIC CHAR*"))
		return -1;

	printf("Call str_LOCALSTR_c \n");
	pRes = str_LOCALSTR_c('c');
	printf("val : %s \n\n",pRes);
	if (strcmp( pRes, "cHIS IS A LOCAL CHAR*"))
		return -1;

	printf("Call c_ITOCHAR_i \n");
	char c_Res = c_ITOCHAR_i(8);
	printf("val : %c \n\n",c_Res);
	if (c_Res != '8')
		return -1;

	printf("Call str_TOLOWER_str \n");
	char* pC = "STRING TO LOWER";
	pRes = str_TOLOWER_str(pC);
	printf("val : %s \n\n",pRes);
	if (strcmp( pRes, "string to lower"))
		return -1;

	printf("Call str_TOLOWER_pv \n");
	void* pV = (void*)pC;
	pRes = str_TOLOWER_pv(pV);
	printf("val : %s \n\n",pRes);
	if (strcmp( pRes, "string to lower"))
		return -1;

	return 0;
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



int Test_CFunctionPointer()
{
	/*
	i_CALLFCT_pv_i
	i_CALLFCT_I_DIV_I_i
	i_CALLFCT_I_MULT_I_i
	i_CALLFCT_I_ADD_I_i
	*/


	printf("Call i_CALLFCT_pv_i \n");
	//ptr_on_i_FCT_i pFct;
	//int res = i_CALLFCT_pv_i(pFct, 3);

	//printf("val : %d \n\n",res);
	//if (res == -1)
	//if (strcmp( pRes, "string to lower"))
	//	return -1;


	return 0;
}

