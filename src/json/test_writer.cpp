#include "json/jsonWriter.h"
#include "json/jsonReader.h"
#include <test_tools/TestAssert.h>
#include <string>
#include <sstream>
#include <iostream>
#include "test_data.h"
void Test_writer()
{
  {
    std::stringstream aStream;
    JSONWriter aWriter(aStream);
    {
      JSON_MAP(aWriter);
      aWriter.pair("test1",1u);
      aWriter.pair("test2","2");
      aWriter.key("test3");
      aWriter.valueNull();
    }
    //std::cout << aStream.str() << std::endl;
    {
      JSONReader aReader;
      auto root = aReader.parse(aStream.str());
      CPPUNIT_ASSERT(root.getInteger("test1")==1);
      CPPUNIT_ASSERT(root.getString("test2")=="2");
      CPPUNIT_ASSERT(root.isNull("test3"));
    }
  }
  {
    std::stringstream aStream;
    JSONWriter aWriter(aStream);
    {
      JSON_LIST(aWriter);
      aWriter.value("test1");
      aWriter.value(2u);
      aWriter.valueNull();
    }
    //std::cout << aStream.str() << std::endl;
    {
      JSONReader aReader;
      auto root = aReader.parse(aStream.str());
    }
  }
  {
    std::stringstream aStream;
    JSONWriter aWriter(aStream);
    {
      JSON_LIST(aWriter);
      aWriter.value("test1");
      aWriter.value(2u);
      aWriter.valueNull();{
        JSON_MAP( aWriter );
        aWriter.pair( "test","3");
        aWriter.key("test2");
        {
          JSON_LIST(aWriter);
        }
      }
    }
    //std::cout << aStream.str() << std::endl;
    {
      JSONReader aReader;
      auto root = aReader.parse(aStream.str());
    }
  }
}

int main()
{
	TEST_INIT();
	
	Test_writer();

	return TEST_HAVEERROR();
}
