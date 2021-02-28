#pragma once
#include <string>
#include <string_view>
#include <ostream>
#include <vector>
#include <unordered_map>
namespace dbproxy
{
  class DBProxy; class ColumnDescr;
}
class JSONWriter;
namespace wasabi{
  namespace metadata{
    using namespace dbproxy;
    class Column{
      friend class Table;
    public:
      enum DataType{
        String=1,
        Numeric=2,
        DateTime=3
      };
      enum Aggregation{
        None=0,
        Sum=1,
        Max=2,
        Min=3,
        Count=4,
      };
      //MU: tbd Column(const Column&)=delete;
      //MU: tbd Column& operator=(const Column&) = delete;
      const std::string& getName()const{return itsName;};
      Aggregation getAggregation()const{return itsAggregation;};
      const std::string& getSQLName()const{return itsSQLName;};
      DataType getDataType()const{return itsDataType;};
      void write(JSONWriter& theWriter)const;
    private:
      explicit Column(const dbproxy::ColumnDescr& theColumnDesc);
      DataType itsDataType;
      Aggregation itsAggregation;
      std::string itsName;
      std::string itsSQLName;
    };
    std::ostream& operator<<(std::ostream& theStream, const Column& theColumn);
    class Table{
      friend class Catalog;
    public:
      /*MU: make private later*/ void addColumn(const dbproxy::ColumnDescr& theColumnDesc);
      /*MU:make private later*/ explicit Table( std::string_view theName);
      //MU: tbd Table(const Table&)=delete;
      //MU: tbd Table& operator=(const Table&) = delete;
      const std::string& getName()const{return itsName;};
      const std::string& getSQLName()const{return itsSQLName;};
      const std::vector<Column>& getColumns()const{return itsColumns;};
      void write(JSONWriter& theWriter)const;
    private:
      std::string itsName;
      std::string itsSQLName;
      std::vector<Column> itsColumns;
    };
    std::ostream& operator<<(std::ostream& theStream, const Table& theTable);
    class Catalog{
    public:
      Catalog( const DBProxy& theConnection);
      const std::vector<std::string>& getTableNames()const;
      const Table& getTable(std::string_view theName)const;
      Catalog(const Catalog&)=delete;
      Catalog& operator=(const Catalog&) = delete;
      void write(JSONWriter& theWriter)const;
    private:
      std::vector<std::string> itsTableNames;
      std::unordered_map<std::string,Table> itsTables;
    };
    std::ostream& operator<<(std::ostream& theStream, const Catalog& theCatalog);
  }
}
