#include "Cube.h"
#include "Dimension.h"

#include <InA_query_model/DataSource.h>
#include <metadata_enrichment/Tables.h>

#include <memory>
#include <iostream>


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

        m_dimensions.push_back(std::make_unique<Dimension>("agg1_id", "index"));
        m_dimensions.push_back(std::make_unique<Dimension>("Yr", "Year", eAxe::Rows));
        m_dimensions.push_back(std::make_unique<Dimension>("Qtr", "Quarter"));
        m_dimensions.push_back(std::make_unique<Dimension>("Month_name", "Month name"));
        m_dimensions.push_back(std::make_unique<Dimension>("Mth", "Month nb"));
        m_dimensions.push_back(std::make_unique<Dimension>("Wk", "Week"));
        m_dimensions.push_back(std::make_unique<Dimension>("City", "City name"));

        std::unique_ptr<Dimension> measuresDim = std::make_unique<DimensionMeasures>("CustomDimension1", "Measures", eAxe::Columns);
        measuresDim->addMember(Member(*measuresDim, "Sales_revenue", "Sales revenue", "Sales revenue"));
        measuresDim->addMember(Member(*measuresDim, "Quantity_sold", "Quantity sold", "Quantity sold"));
        measuresDim->addMember(Member(*measuresDim, "Margin", "Margin Name", "Margin Name"));
        m_dimensions.push_back(std::move(measuresDim));
	 }

     const std::vector<std::unique_ptr<Dimension>>&  Cube::getDimensions() const
     {
         return m_dimensions;
     }
}