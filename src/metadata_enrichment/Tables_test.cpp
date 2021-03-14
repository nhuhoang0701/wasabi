#include <dbproxy/dbproxy.h>

#include <sstream>
#include <ios>
#include <iostream>
#include <memory>
#include <test_tools/TestAssert.h>
#include "metadata_enrichment/Tables.h"
#include <json/jsonReader.h>


using namespace std;
using namespace wasabi::metadata;
using namespace dbproxy;

void testTables()
{
  const string aCnxStr("local:sqlite:onetable_datatype.db");
  const string aTableNameStr("onetable_datatype");
  const Catalog aCatalog(aCnxStr);
  {
    stringstream aStream;
    aStream << aCatalog;
    //cout << "Catalog: " << std::endl << aStream.str() << std::endl;
    JSONReader aReader;
    auto aJSON = aReader.parse(aStream.str());
    CPPUNIT_ASSERT(aJSON.getObject("tables").getObject(aTableNameStr).isString("Name"));
    CPPUNIT_ASSERT(aJSON.getArray("tableNames").isString(1));
  }
}
