#include "test_tools/TestAssert.h"
#include "metadata_enrichtment_test.h"
int main()
{
  TEST_INIT();
  testServerInfo();
  return TEST_HAVEERROR();
}
