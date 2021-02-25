#pragma once
#include <string>
#include <string_view>
#include <ostream>
namespace wasabi{
  namespace metadata{
    class Catalog{
    public:
      Catalog( DBConnection& theConnection);
      const std::vector<std::string> > getTableNames()const;
      const Table& getTable(string_view theName)const;
    }
    class Table{
    public:
      Table(const Table&)=delete;
      Table& operator=(const Table&) = delete;
      const std::string& getName();
      const std::string& getSQLName()const;
      const std::vector<Column>& getColumns()const;
    };
    class Column{
    public:
      const std::string& getName()const;
      Aggregation getAggregation()const;
      const std::string& getSQLName()const;
      DataType getDataType()const;
    };
    enum DataType{
      Uninitialized=-1;
      String=1;
      Numeric=2;
      DateTime=3;
    }
    enum Aggretation{
      Uninitialized=-1
      None=0;
      Sum=1;
      Max=2;
      Min=3;
      Count=4;
  }
}
