#pragma once
#include <string>
#include <string_view>
#include <ostream>
namespace wasabi{
  namespace metadata{
    using namespace std;
    class ServerInfo{
    public:
      ServerInfo(string_view theLanguage);
      const string& getClient()const;
      const string& DataBaseManagementSystem()const;
      const string& ReentranceTicket()const;
      const string& SystemId()const;
      const string& UserLanguageCode()const;
      ServerInfo(const ServerInfo&)=delete;
      ServerInfo& operator=(const ServerInfo&) = delete;
    private:
      const string itsLanguage;
      const string itsDBMS;
      const string itsSystemId;
      const string itsReentranceTicket;
    };
    ostream& operator<<(ostream& theStream, const ServerInfo& theServerInfo);
  }
}
