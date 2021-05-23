#include "Cube.h"
#include "Dimension.h"

#include <InA_query_model/DataSource.h>
#include <metadata_enrichment/Tables.h>

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
			throw std::runtime_error("write cube on $$DataSource$$ NYI");
        
        {
            const std::string& cnxString = m_datasource.getPackageName();
            std::shared_ptr<wasabi::metadata::Catalog> catalog = std::shared_ptr<wasabi::metadata::Catalog>(new wasabi::metadata::Catalog(cnxString));

            const std::string& tableName = m_datasource.getObjectName();
            const auto& colNames = catalog->getTable(tableName).getColumnNames();
            {
                for(const auto& colName : colNames)
                {
                    const auto& column = catalog->getTable(tableName).getColumn(colName);
                    //std::cout << "******************" << std::endl;
                    //std::cout << "column.getName():" << column.getName() << std::endl;
                    //std::cout << "column.getSQLName():" << column.getSQLName() << std::endl;
                    //std::cout << "column.getDataType():" << static_cast<std::uint32_t>(column.getDataType()) << std::endl;
                    //std::cout << "column.getAggregation():" << static_cast<std::uint32_t>(column.getAggregation()) << std::endl;
                }
            }
        }

        if(tableName == "Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma" )
        {
            {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[index]", "index");
                dim->addKeyAttribute(Attribute(*dim, "agg1_id", "agg. idx", eAttrType::Key));
            m_dimensions.push_back(std::move(dim));
        }
        {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Year]", "Year", eAxe::Rows);
                dim->addKeyAttribute(Attribute(*dim, "Yr", "Year", eAttrType::Key));
            m_dimensions.push_back(std::move(dim));
        }
        {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Quarter]", "Quarter");
                dim->addKeyAttribute(Attribute(*dim, "Qtr", "Quarter", eAttrType::Key));
            m_dimensions.push_back(std::move(dim));
        }
        {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Month]", "Month");
            dim->addKeyAttribute(Attribute(*dim, "Mth", "Month number", eAttrType::Key));
            dim->addTextAttribute(Attribute(*dim, "Month_name", "Month name", eAttrType::Text));
            m_dimensions.push_back(std::move(dim));
        }
        {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[Week]", "Week");
                dim->addKeyAttribute(Attribute(*dim, "Wk", "Week", eAttrType::Key));
            m_dimensions.push_back(std::move(dim));
        }
        {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[City]", "City");
                dim->addKeyAttribute(Attribute(*dim, "City", "City", eAttrType::Key));
            m_dimensions.push_back(std::move(dim));
        }

        {
            std::unique_ptr<Dimension> measuresDim = std::make_unique<DimensionMeasures>("CustomDimension1", "Measures", eAxe::Columns);            
            measuresDim->addKeyAttribute(Attribute(*measuresDim, "[Measures].[Measures]", "[Measures].[Measures]", eAttrType::Key));
            measuresDim->addTextAttribute(Attribute(*measuresDim, "[Measures].[Name]", "[Measures].[Name]", eAttrType::Text));

            measuresDim->addMember(Member(*measuresDim, "Sales_revenue", "Sales revenue", "Sales revenue"));
            measuresDim->addMember(Member(*measuresDim, "Quantity_sold", "Quantity sold", "Quantity sold"));
            measuresDim->addMember(Member(*measuresDim, "Margin", "Margin", "Margin"));
            m_dimensions.push_back(std::move(measuresDim));
        }
	 }
        else if(tableName == "onetable_datatype" )
        {
            {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[text dimension name]", "text dimension description");
                dim->addKeyAttribute(Attribute(*dim, "text", "text attribut description", eAttrType::Key));
                m_dimensions.push_back(std::move(dim));
            }
            {
                std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("[varchar dimension name]", "varchar dimension description");
                dim->addKeyAttribute(Attribute(*dim, "varchar", "varchar attribut description", eAttrType::Key));
                m_dimensions.push_back(std::move(dim));
            }
            {
                std::unique_ptr<Dimension> measuresDim = std::make_unique<DimensionMeasures>("[Dimension of measures name]", "Dimension of measures description", eAxe::Columns);            
                measuresDim->addKeyAttribute(Attribute(*measuresDim, "[Measures].[Measures]", "[Measures].[Measures]", eAttrType::Key));
                measuresDim->addTextAttribute(Attribute(*measuresDim, "[Measures].[Name]", "[Measures].[Name]", eAttrType::Text));

                measuresDim->addMember(Member(*measuresDim, "integer", "integer", "integer"));
                measuresDim->addMember(Member(*measuresDim, "real", "real", "real"));
                m_dimensions.push_back(std::move(measuresDim));
            }
        }
        else
            throw std::runtime_error("Unknow datasource:"+tableName);
	 }

     const Dimension&  Cube::getDimension(const std::string& name) const
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
         std::cout << __PRETTY_FUNCTION__ << " attributeName:" << attributeName << std::endl;
         for(const auto& dim : m_dimensions)
         {
             for(const auto& attribut : dim->getAttributes())
             {
                std::cout << "attribut_dim:" << attribut.getName() << std::endl;
                 if(attribut.getName() == attributeName)
                 return true;
             }
             for(const auto& member : dim->getMembers())
             {
                std::cout << "member_dim:" << member.getName().second << std::endl;
                 if(member.getUniqueName().second == attributeName)
                    return true;
         }
         }
        std::cout << "containsAttributes:" << false << std::endl;

         return false;
     }
}