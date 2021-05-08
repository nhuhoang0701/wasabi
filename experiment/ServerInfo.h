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
      const string& getDataBaseManagementSystem()const;
      const string& getReentranceTicket()const;
      const string& getSystemId()const;
      const string& getUserLanguageCode()const;
      ServerInfo(const ServerInfo&)=delete;
      ServerInfo& operator=(const ServerInfo&) = delete;
    private:
      const string itsLanguage;
      const string itsDBMS;
      const string itsSystemId;
      const string itsReentranceTicket;
      const string itsClient;
    };
    ostream& operator<<(ostream& theStream, const ServerInfo& theServerInfo);
  }
}
