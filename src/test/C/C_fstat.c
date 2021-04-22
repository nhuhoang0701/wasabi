#include <test_tools/TestAssert.h>

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

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
