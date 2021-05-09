#include "Cube.h"
#include "InA_metadata/Dimension.h"

#include <InA_query_model/DataSource.h>
#include <memory>


namespace ina::metadata
{
	Cube::Cube(const ina::query_model::DataSource& datasource)
	 : m_datasource(datasource)
	 {		
        if(m_datasource.getObjectName() == "$$DataSource$$" )
			throw std::runtime_error("write cube on $$DataSource$$ NYI");

        m_dimensions.push_back(std::make_unique<Dimension>("agg1_id", "index"));
        m_dimensions.push_back(std::make_unique<Dimension>("Yr", "Year", eAxe::Rows));
        m_dimensions.push_back(std::make_unique<Dimension>("Qtr", "Quarter"));
        m_dimensions.push_back(std::make_unique<Dimension>("Month_name", "Month name"));
        m_dimensions.push_back(std::make_unique<Dimension>("Mth", "Month nb"));
        m_dimensions.push_back(std::make_unique<Dimension>("Wk", "Week"));
        m_dimensions.push_back(std::make_unique<Dimension>("City", "City name"));
        m_dimensions.push_back(std::make_unique<DimensionMeasures>("CustomDimension1", "Measures", eAxe::Columns));
	 }

     const std::vector<std::unique_ptr<Dimension>>&  Cube::getDimensions() const
     {
         return m_dimensions;
     }
}