#include "metadata_enrichment/ServerInfo.h"
namespace wasabi{
  namespace metadata{
    ServerInfo::ServerInfo(
      string_view theLanguage
      ):itsLanguage(theLanguage),
        itsDBMS("HDB"),
        itsSystemId("WEB"),
        itsReentranceTicket("<dummy>"){
    }
  }
}
