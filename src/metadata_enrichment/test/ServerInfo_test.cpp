#include <sstream>
#include "metadata_enrichment/ServerInfo.h"
#include "json/jsonReader.h"
void testServerInfo(){
  const ServerInfo aSI("en");
  std::stringstream aStream;
  aStream << aSI;
  JSONReader aReader;
  aReader.parse(aStream.str());
}
