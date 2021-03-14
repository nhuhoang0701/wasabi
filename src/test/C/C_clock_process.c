#include <test_tools/TestAssert.h>

#include <stdio.h>
#include <math.h>          // For pow, sqrt
#include <string.h>        // For memcpy
#include <stdlib.h>        // For malloc and co
#include <unistd.h>
#include <time.h>

//TODO: Centralyse it
#define EXPORT 


EXPORT void CLOCK_GET_TIME()
{

    int result;
    clockid_t clk_id;
    struct timespec tp;


//  clk_id = CLOCK_REALTIME;
//    clk_id = CLOCK_MONOTONIC;
//    clk_id = CLOCK_BOOTTIME;
     clk_id = CLOCK_PROCESS_CPUTIME_ID;
//
      // int clock_gettime(clockid_t clk_id, struct timespec *tp);
      CPPUNIT_ASSERT_EQUAL(0, clock_gettime(clk_id, &tp));
      printf("tp.tv_sec: %lld\n", tp.tv_sec);
      printf("tp.tv_nsec: %ld\n", tp.tv_nsec);

     CPPUNIT_ASSERT_EQUAL(0, result = clock_getres(clk_id, &tp));
     printf("tp.tv_sec: %lld\n", tp.tv_sec);
     printf("tp.tv_nsec: %ld\n", tp.tv_nsec);

}

int main()
{
   TEST_INIT();
   CLOCK_GET_TIME();

   return TEST_HAVEERROR();
}
