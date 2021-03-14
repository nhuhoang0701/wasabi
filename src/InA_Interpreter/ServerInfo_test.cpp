#include <sstream>
#include "ServerInfo.h"

#include <json/jsonReader.h>
#include <iostream>

using namespace wasabi::metadata;

void testServerInfo()
{
  const ServerInfo aSI("en");
  std::stringstream aStream;
  aStream << aSI;
  JSONReader aReader;
  aReader.parse(aStream.str());
  std::cout << "SystemInfo: " << std::endl << aStream.str() << std::endl;
}
