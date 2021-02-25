#pragma once
#include <string>
#include <string_view>
#include <ostream>
#include <vector>
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
      const std::string& getName()const;
      Aggregation getAggregation()const;
      const std::string& getSQLName()const;
      DataType getDataType()const;
    };
    class Table{
    public:
      Table(const Table&)=delete;
      Table& operator=(const Table&) = delete;
      const std::string& getName();
      const std::string& getSQLName()const;
      const std::vector<Column>& getColumns()const;
    };

    class Catalog{
    public:
      Catalog( DBProxy& theConnection);
      const std::vector<std::string>& getTableNames()const;
      const Table& getTable(std::string_view theName)const;
      Catalog(const Catalog&)=delete;
      Catalog& operator=(const Catalog&) = delete;
    private:
      const std::vector<std::string> itsTableNames;
    };
  }
}
