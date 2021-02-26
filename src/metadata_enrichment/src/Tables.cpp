#include "metadata_enrichment/Tables.h"
#include <algorithm>
#include  <stdexcept>
#include "dbproxy.h"
#include "json/jsonWriter.h"
namespace wasabi{
  namespace metadata{
    using namespace std;
    void retrieveTables(
      dbproxy::DBProxy& theProxy,
      vector<string>& theNames,
      unordered_map<string,Table>& theTables
      )
    {
      const auto& aTables = theProxy.getTables();
      transform(
        aTables.begin(),aTables.end(),
        back_inserter(theNames),
        [](const auto& theTableDescr) -> const string& {
          return theTableDescr.getName();
        }
        );
      sort(theNames.begin(), theNames.end());
    }
    Catalog::Catalog(dbproxy::DBProxy& theProxy){
      retrieveTables(theProxy, itsTableNames,itsTables);
    }
    const vector<string>& Catalog::getTableNames()const{
      return itsTableNames;
    };
    const Table& Catalog::getTable(const string& theName)const{
      auto aIt = itsTables.find(theName);
      if (aIt != itsTables.end()){
        return aIt->second;
      }
      throw new out_of_range("table not found");
    };
    ostream& operator<<(ostream& theStream, const Catalog& theCatalog){
      JSONWriter aWriter(theStream);
      JSON_MAP(aWriter);
      {
        aWriter.key("tableNames");
        JSON_LIST(aWriter);
        for_each(
          theCatalog.getTableNames().begin(), theCatalog.getTableNames().end(),
          [ &aWriter](const string& theString) -> void {
            aWriter.value(theString);
          }
          );
      }
      {
        aWriter.key("tables");
        JSON_LIST(aWriter);
      }
      return theStream;
    };
  }
}
