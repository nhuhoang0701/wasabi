#include "metadata_enrichment/Tables.h"

#include <dbproxy/dbproxy.h>

#include <json/jsonWriter.h>

#include <algorithm>
#include  <stdexcept>
#include <unordered_map>
#include <sstream>
#include <cstdint>

namespace wasabi{
  using namespace std;
  namespace utils{
    using namespace metadata;
    struct ColumnImpl
	{
      ColumnImpl(
                 common::eDataType theDataType,
                 Column::Aggregation theAggregation,
                 const string& theName,
                 const string& theSQLName
                 ):itsDataType(theDataType),
                   itsAggregation(theAggregation),
                   itsName(theName),
                   itsSQLName(theSQLName){
                     itsName = "* " + itsName + " *";
      };
      ColumnImpl(const ColumnImpl&)=delete;
      ColumnImpl& operator=(const ColumnImpl&) = delete;
      common::eDataType itsDataType;
      Column::Aggregation itsAggregation;
      string itsName;
      string itsSQLName;
    };
    struct TableImpl
	{
      TableImpl(const TableImpl&)=delete;
      TableImpl& operator=(const TableImpl&) = delete;
      TableImpl(string_view theName,string_view theSQLName)
		:itsName(theName),itsSQLName(theSQLName)
	{
      }
      string itsName;
      string itsSQLName;
      unordered_map<string, unique_ptr<Column> > itsColumns;
      vector<string> itsColumnNames;
    };
    struct CatalogImpl
	{
      CatalogImpl(){};
      ~CatalogImpl(){};
      CatalogImpl(const CatalogImpl&)=delete;
      CatalogImpl& operator=(const TableImpl&) = delete;
      vector<string> itsTableNames;
      unordered_map<string,unique_ptr<Table> > itsTables;
    };
  }
  namespace metadata
  {
    void retrieveTables(
                        const dbproxy::DBProxy& theProxy,
                        vector<string>& theNames,
                        unordered_map<string,unique_ptr<Table>>& theTables
                        )
    {
      const auto& aTables = theProxy.getTables();
      transform(aTables.begin(),aTables.end(),
                back_inserter(theNames),
                [](const auto& theTableDescr) -> const string&
				{
                  return theTableDescr.getName();
                }
                );
      for_each(
               theNames.begin(), theNames.end(),
               [&theProxy, &theNames, &theTables](const string& theName ) -> void
			   {
                 const dbproxy::TableDescr& aTableDescr = theProxy.getTableDescr(theName);
                 const auto& aCols = aTableDescr.getColumnsDescr();
                 auto anIter = theTables.insert(pair<string,unique_ptr<Table>>(theName,unique_ptr<Table>(new Table(theName)))                                                );
                 if(!anIter.second){
                   stringstream aStream;
                   aStream << "Table " << theName << " not unique!" << endl;
                   throw out_of_range(aStream.str());
                 }
                 Table& aTable = *(anIter.first->second);
                 for_each(
                          aCols.begin(),aCols.end(),
                          [&aTable](const auto& theColumnDescr) -> void {
                            aTable.addColumn(theColumnDescr);
                          }
                          );
               }
               );
      sort(theNames.begin(), theNames.end());
    }
	
    Catalog::Catalog(const std::string& cnxString)
		:itsPimpl(new utils::CatalogImpl())
	{
	  std::shared_ptr<dbproxy::DBProxy> theProxy = dbproxy::DBProxy::getDBProxy(cnxString);
      retrieveTables(*theProxy,itsPimpl->itsTableNames,itsPimpl->itsTables);
    }
    Catalog::~Catalog()
	{
      delete itsPimpl;
    }
    const vector<string>& Catalog::getTableNames()const
	{
      return itsPimpl->itsTableNames;
    };
    const Table& Catalog::getTable(string_view theName)const
	{
      string aName(theName);
      auto aIt = itsPimpl->itsTables.find(aName);
      if (aIt != itsPimpl->itsTables.end())
	  {
        return *(aIt->second);
      }
      throw out_of_range(string("table not found: ") + string(theName));
    };
    void Catalog::write(JSONWriter& theWriter)const
	{
      JSON_MAP(theWriter);
      {
        theWriter.key("tableNames");
        JSON_LIST(theWriter);
        for_each(getTableNames().begin(), getTableNames().end(),
                 [ &theWriter](const string& theString) -> void
				 {
                   theWriter.value(theString);
                 }
                 );
      }
      {
        theWriter.key("tables");
        JSON_MAP(theWriter);
        for_each(getTableNames().begin(), getTableNames().end(),
                 [ this, &theWriter](const string& theName) -> void
				 {
                   theWriter.key(theName);
                   const auto& aTable =getTable(theName);
                   aTable.write(theWriter);
                 }
                 );
      }
    }
    ostream& operator<<(ostream& theStream, const Catalog& theCatalog)
	{
      JSONWriter aWriter(theStream);
      theCatalog.write(aWriter);
      return theStream;
    };
    Table::Table(string_view theName)
		:itsPimpl(new utils::TableImpl(theName,theName))
	{
    };
    const string& Table::getName()const
	{
      return itsPimpl->itsName;
    };
    const string& Table::getSQLName()const
	{
      return itsPimpl->itsSQLName;
    };
    const Column& Table::getColumn(string_view theName)const
	{
      string aName(theName);
      auto aIt = itsPimpl->itsColumns.find(aName);
      if (aIt != itsPimpl->itsColumns.end())
	  {
        return *(aIt->second);
      }
      throw out_of_range("column not found");
    }
    const vector< string >& Table::getColumnNames()const
	{
      return itsPimpl->itsColumnNames;
    };
    void Table::addColumn( const dbproxy::ColumnDescr& theDesc)
	{
      auto anIter = itsPimpl->itsColumns.insert(make_pair(theDesc.getName(),unique_ptr<Column>(new Column(theDesc))));
      if(!anIter.second)
	  {
        stringstream aStream;
        aStream << "Column " << theDesc.getName() << " not unique!" << endl;
        throw out_of_range(aStream.str());
      }
      itsPimpl->itsColumnNames.push_back(theDesc.getName());
    }
    void Table::write(JSONWriter& theWriter)const
	{
      JSON_MAP(theWriter);
      theWriter.pair("Name", getName());
      theWriter.pair("SQLName", getSQLName());
      theWriter.key("Columns");
      JSON_LIST(theWriter);
      for_each(getColumnNames().begin(), getColumnNames().end(),
               [ this, &theWriter](const string& theColName)->void
			   {
                 getColumn(theColName).write(theWriter);
               });
    }
	
    ostream& operator<<(ostream& theStream, const Table& theTable)
	{
      JSONWriter aWriter(theStream);
      theTable.write(aWriter);
      return theStream;
    }
	
    Column::Aggregation calcAggreation( common::eDataType theType)
	{
      if( theType == common::eDataType::Numeric )
	  {
        return Column::Aggregation::Sum;
      } else
	  {
        return Column::Aggregation::None;
      }
      stringstream aStream;
      aStream << "datatype " << static_cast<uint8_t>(theType) << " not found" << endl;
      throw out_of_range(aStream.str());
    }
    common::eDataType calcDataType( const string& theSQLType)
	{
      if( theSQLType == "TEXT"
          || theSQLType.find("CHAR") != std::string::npos)
        {
          return common::eDataType::String;
        }
      else if( theSQLType.find("TIME") != std::string::npos
               || theSQLType.find("DATE") != std::string::npos )
        {
          return common::eDataType::DateTime;
        }
      else if( theSQLType == "REAL"
               || theSQLType == "DOUBLE"
               || theSQLType.find("INT") != std::string::npos
               || theSQLType.find("NUMERIC") != std::string::npos )
		{
			return common::eDataType::Numeric;
		}
      throw out_of_range("datatype " + theSQLType +" not found");
    }
    Column::Column(const dbproxy::ColumnDescr& theDesc)
		:itsPimpl(new utils::ColumnImpl(
									calcDataType(theDesc.getDataType()) ,
									calcAggreation(calcDataType(theDesc.getDataType())),
									theDesc.getName(),
									theDesc.getName()
									) )
	{
    }
    const string& Column::getName()const
	{
      return itsPimpl->itsName;
    };
    Column::Aggregation Column::getAggregation()const
	{
      return itsPimpl->itsAggregation;
    };
    const string& Column::getSQLName()const
	{
      return itsPimpl->itsSQLName;
    };
    common::eDataType Column::getDataType()const
	{
      return itsPimpl->itsDataType;
    };
    void Column::write(JSONWriter& theWriter)const
	{
      JSON_MAP(theWriter);
      theWriter.pair("Name", getName());
      theWriter.pair("SQLName", getSQLName());
      theWriter.pair("Aggregation", static_cast<std::uint32_t>(getAggregation()));
      theWriter.pair("DataType",static_cast<std::uint32_t>(getDataType()));
    }
    ostream& operator<<(ostream& theStream, const Column& theColumn)
	{
      JSONWriter aWriter(theStream);
      theColumn.write(aWriter);
      return theStream;
    }
  }
}
