#pragma once
#include <string>
#include <string_view>
#include <ostream>
#include <vector>
#include <unordered_map>
namespace dbproxy
{
  class DBProxy;
}
namespace wasabi{
  namespace metadata{
    using namespace dbproxy;
    class Column{
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
      const std::string& getName()const{return itsName;};
      Aggregation getAggregation()const{return itsAggregation;};
      const std::string& getSQLName()const{return itsSQLName;};
      DataType getDataType()const{return itsDataType;};
    private:
      DataType itsDataType;
      std::string itsName;
      std::string itsSQLName;
      Aggregation itsAggregation;
    };
    std::ostream& operator<<(std::ostream& theStream, const Column& theColumn);
    class Table{
    public:
      Table(const Table&)=delete;
      Table& operator=(const Table&) = delete;
      const std::string& getName()const{return itsName;};
      const std::string& getSQLName()const{return itsSQLName;};
      const std::vector<Column>& getColumns()const{return itsColumns;};
    private:
      std::string itsName;
      std::string itsSQLName;
      std::vector<Column> itsColumns;
    };
    std::ostream& operator<<(std::ostream& theStream, const Table& theTable);
    class Catalog{
    public:
      Catalog( DBProxy& theConnection);
      const std::vector<std::string>& getTableNames()const;
      const Table& getTable(const std::string& theName)const;
      Catalog(const Catalog&)=delete;
      Catalog& operator=(const Catalog&) = delete;
    private:
      std::vector<std::string> itsTableNames;
      std::unordered_map<std::string,Table> itsTables;
    };
    std::ostream& operator<<(std::ostream& theStream, const Catalog& theCatalog);
  }
}
