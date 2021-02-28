#include "metadata_enrichment/Tables.h"
#include <algorithm>
#include  <stdexcept>
#include <sstream>
#include "dbproxy.h"
#include "json/jsonWriter.h"
namespace wasabi{
  namespace metadata{
    using namespace std;
    void retrieveTables(
                        const dbproxy::DBProxy& theProxy,
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
      for_each(
               theNames.begin(), theNames.end(),
               [&theProxy, &theNames, &theTables](const string& theName ) -> void {
                 const TableDescr& aTableDescr = theProxy.getTableDescr(theName);
                 const auto& aCols = aTableDescr.getColumnsDescr();
                 Table aTable(theName);
                 for_each(
                          aCols.begin(),aCols.end(),
                          [&aTable](const auto& theColumnDescr) -> void {
                            aTable.addColumn(theColumnDescr);
                          }
                          );
                 theTables.insert(make_pair(theName, aTable));
               }
               );
      sort(theNames.begin(), theNames.end());
    }
    Catalog::Catalog(const dbproxy::DBProxy& theProxy){
      retrieveTables(theProxy, itsTableNames,itsTables);
    }
    const vector<string>& Catalog::getTableNames()const{
      return itsTableNames;
    };
    const Table& Catalog::getTable(string_view theName)const{
      string aName(theName);
      auto aIt = itsTables.find(aName);
      if (aIt != itsTables.end()){
        return aIt->second;
      }
      throw out_of_range("table not found");
    };
    void Catalog::write(JSONWriter& theWriter)const{
      JSON_MAP(theWriter);
      {
        theWriter.key("tableNames");
        JSON_LIST(theWriter);
        for_each(
                 getTableNames().begin(), getTableNames().end(),
                 [ &theWriter](const string& theString) -> void {
                   theWriter.value(theString);
                 }
                 );
      }
      {
        theWriter.key("tables");
        JSON_MAP(theWriter);
        for_each(
                 getTableNames().begin(), getTableNames().end(),
                 [ this, &theWriter](
                                     const string& theName
                                     ) -> void {
                   theWriter.key(theName);
                   const auto & aTable =getTable(theName);
                   aTable.write(theWriter);
                 }
                 );
      }
    }
    ostream& operator<<(ostream& theStream, const Catalog& theCatalog){
      JSONWriter aWriter(theStream);
      theCatalog.write(aWriter);
      return theStream;
    };
    Table::Table(string_view theName):itsName(theName),itsSQLName(theName){};
    void Table::addColumn( const dbproxy::ColumnDescr& theDesc){
      itsColumns.push_back(Column(theDesc));
    }
    void Table::write(JSONWriter& theWriter)const{
      JSON_MAP(theWriter);
      theWriter.pair("Name", getName());
      theWriter.pair("SQLName", getSQLName());
      theWriter.key("Columns");
      JSON_LIST(theWriter);
      for_each(
               getColumns().begin(), getColumns().end(),
               [ &theWriter](const Column& theCol)->void{
                 theCol.write(theWriter);
               });
    }
    ostream& operator<<(ostream& theStream, const Table& theTable){
      JSONWriter aWriter(theStream);
      theTable.write(aWriter);
      return theStream;
    }
    Column::Aggregation calcAggreation( Column::DataType theType){
      if( theType == Column::DataType::Numeric ){
        return Column::Aggregation::Sum;
      } else {
        return Column::Aggregation::None;
      }
      stringstream aStream;
      aStream << "datatype " << theType << " not found" << endl;
      throw out_of_range(aStream.str());
    }
    Column::DataType calcDataType( const string& theType){
      if( theType == "TEXT"
          || theType.find("CHAR") != std::string::npos)
        {
          return Column::DataType::String;
        }
      else if( theType.find("TIME") != std::string::npos
               || theType.find("DATE") != std::string::npos )
        {
          return Column::DataType::DateTime;
        }
      else if( theType == "REAL"
               || theType == "DOUBLE"
               || theType.find("INT") != std::string::npos
               || theType.find("NUMERIC") != std::string::npos )
        {
          return Column::DataType::Numeric;
        }
      throw out_of_range("datatype " + theType +" not found");
    }
    Column::Column(
                   const dbproxy::ColumnDescr& theDesc
                   ):itsDataType( calcDataType(theDesc.getDataType()) ),
                     itsAggregation( calcAggreation(itsDataType)),
                     itsName(theDesc.getName()),
                     itsSQLName(theDesc.getName())
    {}
    void Column::write(JSONWriter& theWriter)const{
      JSON_MAP(theWriter);
      theWriter.pair("Name", getName());
      theWriter.pair("SQLName", getSQLName());
      theWriter.pair("Aggregation", getAggregation());
      theWriter.pair("DataType",getDataType());
    }
    ostream& operator<<(ostream& theStream, const Column& theColumn){
      JSONWriter aWriter(theStream);
      theColumn.write(aWriter);
      return theStream;
    }
  }
}
