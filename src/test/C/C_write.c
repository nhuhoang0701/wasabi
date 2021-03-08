#include "test_tools/TestAssert.h"

#include <stdio.h>         // For printf
#include <math.h>          // For pow, sqrt
#include <string.h>        // For memcpy
#include <stdlib.h>        // For malloc and co

//TODO: Centralyse it
#define EXPORT 

char buf[1024];

EXPORT char* str_OPENFILE_WRITE_READ_str(const char* file)
{
	char out[10] = "coucou";

	for(size_t i = 0; i < 1024; ++i)
		buf[i] = 0;

	FILE* fp = fopen (file, "w");
	if(fp == 0)
		return "Error during fopen call";
	int res = fwrite(out, 1, 6, fp);
	fclose(fp);

	fp = fopen (file, "r");
	if(fp == 0)
		return "Error during fopen call";
	res = fread(buf, 1, sizeof buf, fp);
	if(res < 0)
		return "Error during fread call";

	if (ferror(fp))
		return "Error during ferror call";

	fclose(fp);

	return buf;
}

int main()
{
   TEST_INIT();
   CPPUNIT_ASSERT_EQUAL_STR(str_OPENFILE_WRITE_READ_str("./resources/text.out.txt"), "coucou");

   return TEST_HAVEERROR();
}
