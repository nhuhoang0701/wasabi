#include <test_tools/TestAssert.h>

#include <stdio.h>         // For printf
#include <math.h>          // For pow, sqrt
#include <string.h>        // For memcpy
#include <stdlib.h>        // For malloc and co

//TODO: Centralyse it
#define EXPORT 

char buf[1024];


EXPORT char* str_OPENFILE_READ_str(const char* file)
{
	for(size_t i = 0; i < 1024; ++i)
		buf[i] = 0;

	FILE* fp = fopen (file, "r");
	CPPUNIT_ASSERT_MSG(fp != 0,  "Error during fopen call");
	int res = fread(buf, 1, sizeof buf, fp);
	CPPUNIT_ASSERT_MSG(res >= 0,  "Error during fread call");

	res = ferror(fp);
	CPPUNIT_ASSERT_MSG(res >= 0,"Error during ferror call");

	res = fclose(fp);
	CPPUNIT_ASSERT_MSG(res >= 0,"Error during fclose call");

	return buf;
}

int main()
{
   TEST_INIT();
   CPPUNIT_ASSERT_EQUAL_STR(str_OPENFILE_READ_str("./resources/text.txt"), "Hello from text.txt");

   return TEST_HAVEERROR();
}
