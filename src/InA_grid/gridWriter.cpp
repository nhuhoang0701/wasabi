#include "grid.h"

#include <InA_query_model/Query.h>
#include <calculator/cube.h>

#include <json/jsonWriter.h>

#include <iostream>

namespace ina::grid
{
	void                        writeDimensions(const Grid& grid, JSONWriter& writer, const std::vector<const ina::query_model::Dimension*>& dims);
	size_t                      writeTuples(const Grid& grid, JSONWriter& writer, const calculator::eAxe& axis, const std::vector<const ina::query_model::Dimension*>& dims);
	std::pair<size_t, size_t>   writeCells(const Grid& grid, JSONWriter& writer);

    void write(const ina::grid::Grid& grid, JSONWriter& writer)
    {
        JSON_MAP(writer);
        {
            writer.pair("ResultFormat", "Version2");
            writer.key("Axes");
            {
                JSON_LIST(writer);
                if(!grid.getRowDims().empty())
                {
                    JSON_MAP(writer);
                    writer.pair("Name", "ROWS");
                    writeDimensions(grid, writer, grid.getRowDims());

                    size_t tuplesCount = writeTuples(grid, writer, calculator::eAxe::Row, grid.getRowDims());
                    writer.pair("TupleCount", static_cast<uint32_t>(tuplesCount));
                }
                if(!grid.getColDims().empty())
                {
                    JSON_MAP(writer);
                    writer.pair("Name", "COLUMNS");
                    writeDimensions(grid, writer, grid.getColDims());

                    size_t tuplesCount = writeTuples(grid, writer, calculator::eAxe::Column, grid.getColDims());
                    writer.pair("TupleCount", static_cast<uint32_t>(tuplesCount));
                }
            }

            auto cellsSize = writeCells(grid, writer);
            writer.key("CellArraySizes");
            {
                JSON_LIST(writer);
                writer.value(static_cast<uint32_t>(cellsSize.first));
                writer.value(static_cast<uint32_t>(cellsSize.second));
            }
            writer.pair("ResultSetState", 0u);
            writer.pair("HasErrors", true);
            writer.key("Messages");
            {
                JSON_LIST(writer);
                JSON_MAP(writer);
                writer.pair("Number", 1u);
                writer.pair("Text", "This the text of the Error 1 from Grid");
                writer.pair("Type", "Error");
            }
        }
    }

    void writeDimensions(const Grid& grid, JSONWriter& writer, const std::vector<const ina::query_model::Dimension*>& dims)
    {
        writer.key("Dimensions");
        JSON_LIST(writer);
        size_t idxDim = 0;
        for(const auto& dim : dims)
        {
            JSON_MAP(writer);
            writer.pair("Name", dim->getName());
            {
                writer.key("Attributes");
                JSON_LIST(writer);
                if(!ina::query_model::Dimension::isDimensionOfMeasures(*dim) )
                {
                    JSON_MAP(writer);
                    writer.pair("Name", dim->getName());
                    writer.key("Values");
                    {
                        JSON_LIST(writer);
                        const auto& col = *grid.getCube().getStorage().getColumn(dim->getName());
                        for(size_t valueIdx = 0; valueIdx < col.getNumberOfValues(); valueIdx++)
                        {
                            const auto& data = col.getValueAtValueIdx(valueIdx);
                            switch (col.getDataType())
                            {
                            case calculator::eDataType::String:
                            {
                                writer.value(std::get<std::string>(data));
                                break;
                            }
                            case calculator::eDataType::Number:
                            {
                                writer.value(std::get<double>(data));
                                break;
                            }
                            default:
                                throw std::runtime_error("InA_Interpreter => Unsupported datatype.");
                            }
                        }
                    }
                }
                else			
                {
                    {
                        JSON_MAP(writer);
                        writer.pair("Name", "[Measures].[Name]");
                        writer.pair("IsKey", false);
                        writer.key("Values");
                        {
                            JSON_LIST(writer);
                            for(const auto& member : grid.getQuery().getDefinition().getVisibleMembers(*dim))
                            {
                            #ifdef DEBUG
                                writer.value(member.getName() + " (val.[M][N])");
                            #else
                                writer.value(member.getName());
                            #endif
                            }
                        }
                    }
                    {
                        JSON_MAP(writer);
                        writer.pair("Name", "[Measures].[Measures]");
                        writer.pair("IsKey", true);
                        writer.key("Values");
                        {
                            JSON_LIST(writer);
                            for(const auto& member : grid.getQuery().getDefinition().getVisibleMembers(*dim))
                                writer.value(member.getName());
                        }
                    }
                }
            }
            idxDim++;
        }
    }

    size_t writeTuples(const Grid& grid, JSONWriter& writer, const calculator::eAxe& eAxe, const std::vector<const ina::query_model::Dimension*>& dims)
    {
        //std::cout << "**************************************\n";
        //std::cout << "writeTuples\n";
        const ina::query_model::Dimension* measDim = nullptr;
        for(const auto& dimension : dims)
        {
            //std::cout << " " << dimension->getName() << " | ";
            if(ina::query_model::Dimension::isDimensionOfMeasures(*dimension) )
                measDim = dimension;
        }
        //std::cout << "\n";

        auto const& axis = grid.getCube().getAxe(eAxe);
        size_t tuplesCount = axis.getCardinality();
        if(measDim != nullptr)
        {
            if(tuplesCount == 0) // Only dim meas on this axe
                tuplesCount = grid.getQuery().getDefinition().getVisibleMembers(*measDim).size();
            else
                tuplesCount *= grid.getQuery().getDefinition().getVisibleMembers(*measDim).size();
        }
        //std::cout << "tuplesCount: " << tuplesCount << "\n";

        writer.key("Tuples");
        {
            JSON_LIST(writer);
            for(auto dimension : dims)
            {
                JSON_MAP(writer);
                {
                    writer.key("MemberIndexes");
                    {
                        JSON_MAP(writer);
                        writer.pair("Encoding","None");
                        writer.key("Values");
                        {
                            JSON_LIST(writer);
                            {
                                if(ina::query_model::Dimension::isDimensionOfMeasures(*dimension) )
                                {
                                    size_t maxRows = axis.getCardinality();
                                    if(maxRows == 0)
                                        maxRows = 1;
                                    //std::cout << "maxRows: " << maxRows << "\n";
                                    const auto& member = grid.getQuery().getDefinition().getVisibleMembers(*dimension);
                                    //std::cout << "member.size(): " << member.size() << "\n";
                                    for (size_t rowIndex = 0; rowIndex < maxRows; rowIndex++)
                                    {
                                        for(size_t i = 0; i < member.size(); i++)
                                        {
                                            writer.value(static_cast<uint32_t>(i));
                                        }
                                    }
                                }
                                else
                                {
                                    //std::cout << "axis.getCardinality(): " << axis.getCardinality() << "\n";
                                    for (size_t rowIndex = 0; rowIndex < axis.getCardinality(); rowIndex++)
                                    {
                                        const auto idx = static_cast<uint32_t>(axis.getValueIndex(dimension->getName(), rowIndex));
                                        if(measDim)
                                        {
                                            const auto& member = grid.getQuery().getDefinition().getVisibleMembers(*measDim);
                                            for(size_t i = 0; i < member.size(); i++)
                                                writer.value(idx);
                                        }
                                        else
                                            writer.value(idx);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        return tuplesCount;
    }

    std::pair<size_t, size_t> writeCells(const Grid& grid, JSONWriter& writer)
    {
        writer.key("Cells");
        {
            JSON_MAP(writer);
            writer.key("Values");
            {
                JSON_MAP(writer);
                writer.pair("Encoding", "None");
                writer.key("Values");
                {
                    JSON_LIST(writer);	
                    for(size_t rowIndex = 0; rowIndex < grid.getCube().getBody().getRowNbrs(); rowIndex++)
                    {
                        for(size_t colIndex = 0; colIndex < grid.getCube().getBody().getColNbrs(); colIndex++)
                        {
                            for(const auto& measure : grid.getCube().getBody())
                            {
                                const auto& data = grid.getCube().getBody().getValue(measure.getName(), colIndex, rowIndex);
                                switch (grid.getCube().getBody().getValueDatatype(measure.getName()))
                                {
                                case calculator::eDataType::String:
                                {
                                    writer.value(std::get<std::string>(data));
                                    break;
                                }
                                case calculator::eDataType::Number:
                                {
                                    writer.value(std::get<double>(data));
                                    break;
                                }
                                default:
                                    throw std::runtime_error("InA_Interpreter => Unsupported datatype.");
                                }
                            }
                        }
                    }
                }
            }
            //TODO: Not sure why this is needed
            writer.key("ValuesFormatted");
            {
                std::cerr << "WASABI: ERROR: Use harcoded format to dislay ValuesFormatted." << std::endl;
                JSON_MAP(writer);
                writer.pair("Encoding", "None");
                writer.key("Values");
                {
                    JSON_LIST(writer);	
                    for(size_t rowIndex = 0; rowIndex < grid.getCube().getBody().getRowNbrs(); rowIndex++)
                    {
                        for(size_t colIndex = 0; colIndex < grid.getCube().getBody().getColNbrs(); colIndex++)
                        {
                            for(const auto& dim : grid.getCube().getBody())
                            {
                                const auto& data = grid.getCube().getBody().getValue(dim.getName(), colIndex, rowIndex);
                                switch (grid.getCube().getBody().getValueDatatype(dim.getName()))
                                {
                                case calculator::eDataType::String:
                                {
                                    writer.value(std::get<std::string>(data));
                                    break;
                                }
                                case calculator::eDataType::Number:
                                {
                                    writer.value(std::get<double>(data));
                                    break;
                                }
                                default:
                                    throw std::runtime_error("InA_Interpreter => Unsupported datatype.");
                                }
                            }
                        }
                    }
                }
            }
            writer.key("CellValueTypes");
            {
                JSON_MAP(writer);
                writer.pair("Encoding", "None");
                writer.key("Values");
                {
                    JSON_LIST(writer);	
                    for(size_t rowIndex = 0; rowIndex < grid.getCube().getBody().getRowNbrs(); rowIndex++)
                    {
                        for(size_t colIndex = 0; colIndex < grid.getCube().getBody().getColNbrs(); colIndex++)
                        {
                            for(const auto& measName : grid.getCube().getBody())
                            {
                                const auto& data = grid.getCube().getBody().getValue(measName.getName(), colIndex, rowIndex);
                                switch (grid.getCube().getBody().getValueDatatype(measName.getName()))
                                {
                                case calculator::eDataType::String:
                                {
                                    writer.value(4u);
                                    break;
                                }
                                case calculator::eDataType::Number:
                                {
                                    writer.value(10u);
                                    break;
                                }
                                default:
                                    throw std::runtime_error("InA_Interpreter => Unsupported datatype.");
                                }
                            }
                        }
                    }
                }
            }
        }
        
        std::pair<size_t, size_t> cellsSize = std::make_pair(grid.getCube().getBody().getRowNbrs(), grid.getCube().getBody().getColNbrs());
        if( grid.getMeasDim().getAxe()==ina::query_model::Dimension::eAxe::Rows)
            cellsSize.first *= grid.getQuery().getDefinition().getVisibleMembers(grid.getMeasDim()).size();
        if(grid.getMeasDim().getAxe()==ina::query_model::Dimension::eAxe::Columns)
            cellsSize.second *= grid.getQuery().getDefinition().getVisibleMembers(grid.getMeasDim()).size();

        return cellsSize;
    }
}
