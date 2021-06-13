#include "Cube.h"
#include "Dimension.h"
#include "InA_metadata/Member.h"
#include "common/data.h"

#include <InA_query_model/DataSource.h>
#include <metadata_enrichment/Tables.h>

#include <common/Log.h>

#include <memory>
#include <iostream>
#include <stdexcept>


namespace ina::metadata
{
	Cube::Cube(const ina::query_model::DataSource& datasource)
	 : m_datasource(datasource)
	 {
        const std::string& tableName = m_datasource.getObjectName();
        if(tableName == "$$DataSource$$" )
        {
            {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("Type", "Type");
                dim->addKeyAttribute(Attribute(*dim, "Type.KEY", "", eAttrType::Key, common::eDataType::String));
                m_dimensions.push_back(std::move(dim));
            }
            {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("SchemaName", "SchemaName");
                dim->addKeyAttribute(Attribute(*dim, "SchemaName.KEY", "", eAttrType::Key, common::eDataType::String));
                m_dimensions.push_back(std::move(dim));
            }
            {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("ObjectName", "ObjectName");
                dim->addKeyAttribute(Attribute(*dim, "ObjectName.KEY", "", eAttrType::Key, common::eDataType::String));
                dim->addTextAttribute(Attribute(*dim, "ObjectName.LONG_TEXT", "", eAttrType::Text, common::eDataType::String));
                m_dimensions.push_back(std::move(dim));
            }
            {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("PackageName", "PackageName");
                dim->addKeyAttribute(Attribute(*dim, "PackageName.KEY", "", eAttrType::Key, common::eDataType::String));
                m_dimensions.push_back(std::move(dim));
            }
        }
        else
        {
            const std::string& packageName = m_datasource.getPackageName();
            std::shared_ptr<wasabi::metadata::Catalog> catalog = std::shared_ptr<wasabi::metadata::Catalog>(new wasabi::metadata::Catalog(packageName));

            const auto& colNames = catalog->getTable(tableName).getColumnNames();
            {
                std::unique_ptr<Dimension> measuresDim = std::make_unique<DimensionMeasures>("CustomDimension1", "Measures", eAxe::Columns);
                measuresDim->addKeyAttribute(Attribute(*measuresDim, "[Measures].[Measures]", "[Measures].[Measures]", eAttrType::Key, common::eDataType::String));
                measuresDim->addTextAttribute(Attribute(*measuresDim, "[Measures].[Name]", "[Measures].[Name]", eAttrType::Text, common::eDataType::String));

                for(const auto& colName : colNames)
                {
                    const auto& column = catalog->getTable(tableName).getColumn(colName);
                    if(tableName == "Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma" )
                    {
                        if(colName=="agg1_id")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[index]", "index");
                            dim->addKeyAttribute(Attribute(*dim, colName, "agg1.idx", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Yr")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Year]", "Year", eAxe::Rows);
                            dim->addKeyAttribute(Attribute(*dim, colName, "Year", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Qtr")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Quarter]", "Quarter");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Quarter", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Mth")
                        {
                            if(haveDimension("[Month]") == false)
                            {
                                std::unique_ptr<Dimension> dim;
                                dim = std::make_unique<Dimension>("[Month]", "Month");
                                m_dimensions.push_back(std::move(dim));
                            }
                            Dimension& dim = getDimension("[Month]");
                            dim.addKeyAttribute(Attribute(dim, colName, "Month number", eAttrType::Key, column.getDataType()));
                        }
                        else if(colName=="Month_name")
                        {
                            if(haveDimension("[Month]") == false)
                            {
                                std::unique_ptr<Dimension> dim;
                                dim = std::make_unique<Dimension>("[Month]", "Month");
                                m_dimensions.push_back(std::move(dim));
                            }
                            Dimension& dim = getDimension("[Month]");
                            dim.addTextAttribute(Attribute(dim, colName, "Month name", eAttrType::Text, column.getDataType()));
                        }
                        else if(colName=="Wk")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Week]", "Week");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Week", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="City")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[City]", "City");
                            dim->addKeyAttribute(Attribute(*dim, colName, "City", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Store_name")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Store_name]", "Store name");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Store name", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Sales_revenue")
                        {
                            measuresDim->addMember(Member(*measuresDim, colName, "Sales_revenue", "Sales revenue"));
                        }
                        else if(colName=="Quantity_sold")
                        {
                            measuresDim->addMember(Member(*measuresDim, colName, "Quantity_sold", "Quantity sold"));
                        }
                        else if(colName=="Margin")
                        {
                            measuresDim->addMember(Member(*measuresDim, colName, "Margin", "Margin"));
                        }
                        else
                            throw std::runtime_error("Unknow column:"+colName);
                    }
                    else if(tableName == "Agg_yr_qt_rn_st_ln_ca_sr" )
                    {
                        if(colName=="agg2_id")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[index]", "index");
                            dim->addKeyAttribute(Attribute(*dim, colName, "agg2.idx", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Yr")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Year]", "Year");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Year", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Qtr")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Quarter]", "Quarter");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Quarter", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="State")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[State]", "State", eAxe::Rows);
                            dim->addKeyAttribute(Attribute(*dim, colName, "State", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Line")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Line]", "Line");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Line", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Category")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Category]", "Category", eAxe::Columns);
                            dim->addKeyAttribute(Attribute(*dim, colName, "Category", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Sales_revenue")
                        {
                            measuresDim->addMember(Member(*measuresDim, colName, "Sales_revenue", "Sales revenue"));
                        }
                        else
                            throw std::runtime_error("Unknow column:"+colName);
                    }
                    /*else if(tableName == "Shop_facts" )
                    {
                        if(colName=="Shop_facts_id")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Shop_facts_id]", "Shop facts id");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Shop_facts_id", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Article_id")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Article id]", "Article id", eAxe::Rows);
                            dim->addKeyAttribute(Attribute(*dim, colName, "Article_id", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Color_code")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Color_code]", "Color code");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Color_code", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Week_id")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Week_id]", "Week id");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Week_id", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Shop_id")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Shop_id]", "Shop id");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Shop_id", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Margin")
                        {
                            measuresDim->addMember(Member(*measuresDim, colName, "Margin", "Margin"));
                        }
                        else if(colName=="Amount_sold")
                        {
                            measuresDim->addMember(Member(*measuresDim, colName, "Amount_sold", "Amount sold"));
                        }
                        else if(colName=="Quantity_sold")
                        {
                            measuresDim->addMember(Member(*measuresDim, colName, "Quantity_sold", "Quantity sold"));
                        }
                        else if(colName=="Invoice_DateTime")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Invoice_DateTime]", "Invoice DateTime");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Invoice_DateTime", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="Invoice_Date")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Invoice_Date]", "Invoice Date");
                            dim->addKeyAttribute(Attribute(*dim, colName, "Invoice_Date", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else
                            throw std::runtime_error("Unknow column:"+colName);
                    }*/
                    else if(tableName == "onetable_datatype" )
                    {
                        if(colName=="text")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[text dimension name]", "text dimension description");
                            dim->addKeyAttribute(Attribute(*dim, colName, "text attribut description", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="varchar")
                        {
                            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[varchar dimension name]", "varchar dimension description");
                            dim->addKeyAttribute(Attribute(*dim, colName, "varchar attribut description", eAttrType::Key, column.getDataType()));
                            m_dimensions.push_back(std::move(dim));
                        }
                        else if(colName=="integer")
                        {
                            measuresDim->addMember(Member(*measuresDim, colName, "integer", "integer"));
                        }
                        else if(colName=="real")
                        {
                            measuresDim->addMember(Member(*measuresDim, colName, "real", "real"));
                        }
                        else
                            throw std::runtime_error("Unknow column:"+colName);
                    }
                    else
                        throw std::runtime_error("Metadata not found for :'"+tableName+"'");
                }
                m_dimensions.push_back(std::move(measuresDim));
            }
        }
	 }

    const ina::query_model::DataSource& Cube::getDataSource() const
    {
        return m_datasource;
    }

     bool Cube::haveDimension(const std::string name) const
     {
         for(const auto& dimension : m_dimensions)
            if(dimension && dimension->getName() == name)
                return true;
         
         return false;
     }

     const Dimension&  Cube::getDimension(const std::string& name) const
     {
        return  const_cast<Cube&>(*this).getDimension(name);
     }

     Dimension&  Cube::getDimension(const std::string& name)
     {
         for(const auto& dimension : m_dimensions)
            if(dimension->getName() == name)
                return *dimension;
         
         throw std::runtime_error("Cube::getDimension : dimension not found: " + name);
     }

     const std::vector<std::unique_ptr<Dimension>>&  Cube::getDimensions() const
     {
         return m_dimensions;
     }

     bool Cube::containsDataSourceColumn(const std::string& attributeName) const
     {
         ScopeDebug sc("Cube::containsDataSourceColumn");
         Logger::debug("attributeName", attributeName);
         for(const auto& dim : m_dimensions)
         {
             for(const auto& attribut : dim->getAttributes())
             {
                Logger::debug("attribut_dim", attribut.getName());
                 if(attribut.getName() == attributeName)
                    return true;
             }
             for(const auto& member : dim->getMembers())
             {
                Logger::debug("member_dim", member.getName().first);
                 if(member.getUniqueName().second == attributeName)
                    return true;
            }
         }
         Logger::debug("containsAttributes", false);

         return false;
     }

     common::eDataType Cube::getDataType(const std::string& attributeName) const
     {
         ScopeDebug sc("Cube::getDataType");
         Logger::debug("attributeName", attributeName);
         common::eDataType dt = common::eDataType::Undefined;
         for(const auto& dim : m_dimensions)
         {
             for(const auto& attribut : dim->getAttributes())
             {
                Logger::debug("attribut_dim", attribut.getName());
                 if(attribut.getName() == attributeName)
                 {
                    dt = attribut.getDataType();
                    Logger::debug("datatype", static_cast<uint16_t>(dt));
                    break;
                 }
             }
             if(dt != common::eDataType::Undefined)
                break;
         }
        if(dt == common::eDataType::Undefined)
            throw std::runtime_error("Missing datatype for :" + attributeName);

         return dt;
     }
}