#include <sstream>
#include <ios>
#include <iostream>
#include <memory>
#include "test_tools/TestAssert.h"


#include "metadata_enrichment/Tables.h"
#include "json/jsonReader.h"
#include "dbproxy.h"

using namespace std;
using namespace wasabi::metadata;
using namespace dbproxy;
void testTables(){
  const string aCnxStr("local:sqlite:efashion.db");
  const string aTableNameStr("Table1");
  auto aDbProxyPtr = DBProxy::getDBProxy(aCnxStr);
  if(!aDbProxyPtr.get()){
    throw ios_base::failure("No database connection");
  }
  auto & aProxy = *aDbProxyPtr;
  const Catalog aCatalog(aProxy);
  {
    stringstream aStream;
    aStream << aCatalog;
    cout << "Catalog: " << std::endl << aStream.str() << std::endl;
    JSONReader aReader;
    auto aJSON = aReader.parse(aStream.str());
    CPPUNIT_ASSERT(aJSON.getObject("tables").getObject("Table1").isString("Name"));
    CPPUNIT_ASSERT(aJSON.getArray("tableNames").isString(1));
  }
}
