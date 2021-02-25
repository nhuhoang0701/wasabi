#include "test_tools/TestAssert.h"

int main()
{
  TEST_INIT();
  testServerInfo();
  return TEST_HAVEERROR();
}
