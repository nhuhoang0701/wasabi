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
                for(auto& colName : colNames)
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

        {
            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("agg1_id", "index");
            dim->addKeyAttribute(Attribute(*dim, dim->getName(), dim->getDescription(), eAttrType::Key));
            m_dimensions.push_back(std::move(dim));
        }
        {
            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("Yr", "Year", eAxe::Rows);
            dim->addKeyAttribute(Attribute(*dim, dim->getName(), dim->getDescription(), eAttrType::Key));
            m_dimensions.push_back(std::move(dim));
        }
        {
            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("Qtr", "Quarter");
            dim->addKeyAttribute(Attribute(*dim, dim->getName(), dim->getDescription(), eAttrType::Key));
            m_dimensions.push_back(std::move(dim));
        }
        /*{
            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("Month_name", "Month name");
            dim->addKeyAttribute(Attribute(*dim, dim->getName(), dim->getDescription()));
            m_dimensions.push_back(std::move(dim));
        }*/
        {
            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("Mth_dim", "Month dim");
            dim->addKeyAttribute(Attribute(*dim, "Mth", "Month number", eAttrType::Key));
            dim->addTextAttribute(Attribute(*dim, "Month_name", "Month name", eAttrType::Text));
            m_dimensions.push_back(std::move(dim));
        }
        {
            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("Wk", "Week number");
            dim->addKeyAttribute(Attribute(*dim, dim->getName(), dim->getDescription(), eAttrType::Key));
            m_dimensions.push_back(std::move(dim));
        }
        {
            std::unique_ptr<Dimension> dim = std::make_unique<Dimension>("City", "City name");
            dim->addKeyAttribute(Attribute(*dim, dim->getName(), dim->getDescription(), eAttrType::Key));
            m_dimensions.push_back(std::move(dim));
        }

        {
            std::unique_ptr<Dimension> measuresDim = std::make_unique<DimensionMeasures>("CustomDimension1", "Measures", eAxe::Columns);            
            measuresDim->addKeyAttribute(Attribute(*measuresDim, "[Measures].[Measures]", "[Measures].[Measures]", eAttrType::Key));
            measuresDim->addTextAttribute(Attribute(*measuresDim, "[Measures].[Name]", "[Measures].[Name]", eAttrType::Text));

            measuresDim->addMember(Member(*measuresDim, "Sales_revenue", "Sales revenue", "Sales revenue"));
            measuresDim->addMember(Member(*measuresDim, "Quantity_sold", "Quantity sold", "Quantity sold"));
            measuresDim->addMember(Member(*measuresDim, "Margin", "Margin Name", "Margin Name"));
            m_dimensions.push_back(std::move(measuresDim));
        }
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

     bool Cube::containsAttributes(const std::string& attributeName) const
     {
         for(const auto& dim : m_dimensions)
         {
             for(const auto& attribut : dim->getAttributes())
             {
                 if(attribut.getName() == attributeName)
                 return true;
             }
         }

         return false;
     }
}