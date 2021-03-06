#pragma once

#include <common/data.h>

#include <string>
#include <string_view>
#include <ostream>
#include <vector>
#include <memory>

namespace wasabi::utils{class ColumnImpl;class TableImpl;class CatalogImpl;class ConnectionsImpl;}

namespace dbproxy {class DBProxy; class ColumnDescr;}

class JSONWriter;
namespace wasabi::metadata
{
  common::eDataType calcDataType( const std::string& theSQLType);

  class Column
  {
    friend class Table;
    public:
    enum class Aggregation
    {
    None=0,
    Sum=1,
    Max=2,
    Min=3,
    Count=4,
    };

    const std::string& getName()const;
    Aggregation        getAggregation()const;
    const std::string& getSQLName()const;
    common::eDataType  getDataType()const;

    void write(JSONWriter& theWriter)const;

    private:
    explicit Column(const dbproxy::ColumnDescr& theColumnDesc);

    std::unique_ptr<wasabi::utils::ColumnImpl> itsPimpl;
  };
  std::ostream& operator<<(std::ostream& theStream, const Column& theColumn);

  class Table
  {
    friend class Catalog;
    public:
    /*MU: make private later*/ void addColumn(const dbproxy::ColumnDescr& theColumnDesc);
    /*MU:make private later*/ explicit Table( std::string_view theName);
    Table(const Table&)=delete;
    Table& operator=(const Table&) = delete;

    const std::string& getName()const;
    const std::string& getSQLName()const;

    const Column&                     getColumn(std::string_view theName)const;
    const std::vector< std::string> & getColumnNames()const;

    void write(JSONWriter& theWriter)const;
    private:
    std::unique_ptr<utils::TableImpl> itsPimpl;
  };

  std::ostream& operator<<(std::ostream& theStream, const Table& theTable);

  class Catalog
  {
      public:
      Catalog(const std::string& cnxString);
      Catalog(const Catalog&)=delete;
      Catalog& operator=(const Catalog&) = delete;

      ~Catalog();

      const std::vector<std::string>& getTableNames()const;
      const Table&                    getTable(std::string_view theName)const;

      void write(JSONWriter& theWriter)const;

      private:
      wasabi::utils::CatalogImpl* itsPimpl;
    };
    std::ostream& operator<<(std::ostream& theStream, const Catalog& theCatalog);


    class Connections
    {
    public:
    Connections();
    Connections(const Connections&)=delete;
    Connections& operator=(const Connections&) = delete;

    ~Connections();

    const std::vector<std::string>& getConnectionStrings()const;

    private:
    std::unique_ptr<wasabi::utils::ConnectionsImpl> itsPimpl;
  };
}