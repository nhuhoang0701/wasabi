#include "metadata_enrichment/ServerInfo.h"
#include "json/jsonWriter.h"
namespace wasabi{
  namespace metadata{
    ServerInfo::ServerInfo(
      string_view theLanguage
      ):itsLanguage(theLanguage),
        itsDBMS("HDB"),
        itsSystemId("WEB"),
        itsReentranceTicket("<dummy>"),
        itsClient("000"){
    }
    const string& ServerInfo::getClient()const{
      return itsClient;
    }
    const string& ServerInfo::getDataBaseManagementSystem()const{
      return itsDBMS;
    }
    const string& ServerInfo::getReentranceTicket()const{
      return itsReentranceTicket;
    }
    const string& ServerInfo::getSystemId()const{
      return itsSystemId;
    }
    const string& ServerInfo::getUserLanguageCode()const{
      return itsSystemId;
    }
    ostream& operator<<(ostream& theStream, const ServerInfo& theServerInfo){
      JSONWriter aWriter(theStream);
      JSON_MAP(aWriter);
      aWriter.pair("",theServerInfo.getClient());
      aWriter.pair("",theServerInfo.getSystemId());
      aWriter.pair("",theServerInfo.getDataBaseManagementSystem());
      aWriter.pair("",theServerInfo.getUserLanguageCode());
      aWriter.pair("",theServerInfo.getReentranceTicket());
      return theStream;
    }
  }
}
