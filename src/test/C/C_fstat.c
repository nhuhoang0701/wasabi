#include <test_tools/TestAssert.h>

#include <stdio.h>         // For printf
#include <math.h>          // For pow, sqrt
#include <string.h>        // For memcpy
#include <stdlib.h>        // For malloc and co
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

//TODO: Centralyse it
#define EXPORT 


int main()
{
   TEST_INIT();

   int fd = open ("./resources/text.txt",O_RDONLY);
   CPPUNIT_ASSERT (fd >= 0);
   struct stat filestat;
   CPPUNIT_ASSERT_EQUAL (fstat(fd, &filestat) , 0);
   CPPUNIT_ASSERT_EQUAL (filestat.st_size, 19);
   close(fd);

   return TEST_HAVEERROR();
}
