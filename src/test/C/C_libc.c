#include "test_tools/TestAssert.h"

#include <stdio.h>         // For printf
#include <math.h>          // For pow, sqrt
#include <string.h>        // For memcpy
#include <stdlib.h>        // For malloc and co

//TODO: Centralyse it
#define EXPORT 

char buf[1024];

EXPORT int i_PRINT_i(int i)
{
	int wr = printf("str_Print_i:printf '%d'\n", i);
	fflush(stdout);

	wr += fprintf(stdout, "str_Print_i:fprintf stdout '%d'\n", i);
	fflush(stdout);

	wr += fprintf(stderr, "str_Print_i:fprintf stderr '%d'\n", i);
	fflush(stderr);

	return wr;
}

EXPORT double d_SQRT_d(double d)
{
	return sqrt(d);
}

EXPORT double d_FLOOR_d(double d)
{
	return floor(d);
}

double (*ptr_on_d_SQRT_d)(double d) = d_SQRT_d;
EXPORT double d_CALLFCT_D_SQRT_D_d(double d)
{
	printf("d_CALLFCT_D_SQRT_D_d: @ptr_on_d_SQRT_d '%p'\n", ptr_on_d_SQRT_d);
	printf("d_CALLFCT_D_SQRT_D_d: @ptr_on_d_SQRT_d '%ld'\n", (size_t)ptr_on_d_SQRT_d);
	fflush(stdout);
	return ptr_on_d_SQRT_d(d);
}

EXPORT void* pv_MALLOC_i(int i)
{
	return malloc(i);
}

EXPORT void v_FREE_pv(void* i)
{
	free(i);
	i = 0;
}

EXPORT int* pi_MALLOCFREE_i(int i)
{
	int* ptr = malloc(i);
	*ptr = i;
	free(ptr);
	return ptr;
}

EXPORT char* pc_HEAPSTR_c(char c)
{
	char* ptr = malloc(6);
	ptr[0] = c;
	ptr[1] = 'e';
	ptr[2] = 'l';
	ptr[3] = 'l';
	ptr[4] = 'o';
	ptr[5] = 0;

	return ptr;
}

EXPORT void v_MEMCPY_vp_vp_i(void* src, void* dst, int sz)
{
	memcpy(dst, src, sz);
}

EXPORT char* str_OPENFILE_READ_str(const char* file)
{
	for(size_t i = 0; i < 1024; ++i)
		buf[i] = 0;

	FILE* fp = fopen (file, "r");
	if(fp == 0)
		return "Error during fopen call";
	int res = fread(buf, 1, sizeof buf, fp);
	if(res < 0)
		return "Error during fread call";

	if (ferror(fp))
		return "Error during ferror call";

	fclose(fp);

	return buf;
}

EXPORT char* str_OPENFILE_SEEK_READ_str_i(const char* file, int offset, int whence)
{
	for(size_t i = 0; i < 1024; ++i)
		buf[i] = 0;

	FILE* fp = fopen (file, "r");
	if(fp == 0)
		return "Error during fopen call";

	if(fseek(fp, offset, whence) < 0)
		return "Error during fseek call";

	if(fread(buf, 1, sizeof buf, fp) < 0)
		return "Error during fread call";

	if (ferror(fp))
		return "Error during ferror call";

	fclose(fp);

	return buf;
}

int main()
{
	CPPUNIT_ASSERT_EQUAL(i_PRINT_i(2),85);
	CPPUNIT_ASSERT_EQUAL(d_SQRT_d(4),2);
	CPPUNIT_ASSERT_EQUAL(d_FLOOR_d(4),4);
	
	void* pint = pv_MALLOC_i(4);
	CPPUNIT_ASSERT_NOTEQUAL(pint,NULL);
	v_FREE_pv(pint);

	pint = pi_MALLOCFREE_i(4);
	CPPUNIT_ASSERT_NOTEQUAL(pint,NULL);

	CPPUNIT_ASSERT_EQUAL_STR(pc_HEAPSTR_c('D'), "Dello");

	char* pcharSrc = "Source";
	char* pcharDest = malloc(10);
	v_MEMCPY_vp_vp_i(pcharSrc,pcharDest,strlen(pcharSrc)+1);
	CPPUNIT_ASSERT_EQUAL_STR(pcharDest, "Source");

	CPPUNIT_ASSERT_EQUAL_STR(str_OPENFILE_READ_str("../resources/text.txt"), "Hello from text.txt");

   return 0;
}
