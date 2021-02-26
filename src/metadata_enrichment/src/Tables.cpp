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
    Catalog::Catalog(dbproxy::DBProxy& theProxy){
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
        JSON_MAP(aWriter);
        bool bFirst = true;
        for_each(
          theCatalog.getTableNames().begin(), theCatalog.getTableNames().end(),
          [ &theCatalog, &aWriter, &theStream, &bFirst](
            const string& theName
            ) -> void {
            if(bFirst){
              bFirst = false;
            }else{
              theStream<<",";
            }
            aWriter.key(theName);
            aWriter.writeColon();
            const auto & aTable =theCatalog.getTable(theName);
            theStream << aTable;
          }
          );
      }
      return theStream;
    };
    Table::Table(string_view theName):itsName(theName),itsSQLName(theName){};
    void Table::addColumn( const dbproxy::ColumnDescr& theDesc){
      itsColumns.push_back(Column(theDesc));
    }
    ostream& operator<<(ostream& theStream, const Table& theTable){
      JSONWriter aWriter(theStream);
      JSON_MAP(aWriter);
      aWriter.pair("Name", theTable.getName());
      aWriter.pair("SQLName", theTable.getSQLName());
      aWriter.key("Columns");
      bool bFirst = true;
      JSON_LIST(aWriter);
      for_each(
        theTable.getColumns().begin(), theTable.getColumns().end(),
        [&theStream, &aWriter, &bFirst](const Column& theCol)->void{
          if(bFirst){
            bFirst = false;
          }else{
            theStream<<",";
          }
          theStream << theCol;
        });
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
      if( theType == "TEXT" ){
        return Column::DataType::String;
      } else if( theType == "REAL" ){
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
    ostream& operator<<(ostream& theStream, const Column& theColumn){
      JSONWriter aWriter(theStream);
      JSON_MAP(aWriter);
      aWriter.pair("Name", theColumn.getName());
      aWriter.pair("SQLName", theColumn.getSQLName());
      aWriter.pair("Aggregation", theColumn.getAggregation());
      aWriter.pair("DataType",theColumn.getDataType());
      return theStream;
    }
  }
}
