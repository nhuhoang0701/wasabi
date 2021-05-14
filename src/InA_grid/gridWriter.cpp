#include "grid.h"

#include <InA_query_model/Query.h>
#include <calculator/cube.h>

#include <cstdint>
#include <json/jsonWriter.h>

#include <iostream>
#include <stdexcept>

namespace ina::grid
{
	void    writeAxe(JSONWriter& writer, const Axis& axis);
	void    writeDimensions(JSONWriter& writer, const Axis& axis);
	void    writeTuples(JSONWriter& writer, const Axis& axis);
	void    writeCells(const Grid& grid, JSONWriter& writer);

    void write(const ina::grid::Grid& grid, JSONWriter& writer)
    {
        JSON_MAP(writer);
        {
            writer.pair("ResultFormat", "Version2");
            /*writer.key("SubSetDescription");
            {
                JSON_MAP(writer);
                writer.pair("ColumnFrom", "ROWS");
                writer.pair("ColumnTo", "ROWS");
                writer.pair("RowFrom", "ROWS");
                writer.pair("RowTo", "ROWS");
            }*/
            writer.key("Axes");
            {
                JSON_LIST(writer);
                writeAxe(writer, grid.m_rowAxe);
                writeAxe(writer, grid.m_colAxe);
            }

            writeCells(grid, writer);

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
    
	void    writeAxe(JSONWriter& writer, const Axis& axis)
    {
        if(!axis.getDimensions().empty())
        {
            JSON_MAP(writer);
            writer.pair("Name", axis.getName());
            writeDimensions(writer, axis);
            writeTuples(writer, axis);
        }
    }

    void writeDimensions(JSONWriter& writer, const Axis& axis)
    {
        writer.key("Dimensions");
        JSON_LIST(writer);
        size_t idxDim = 0;
        for(const auto& dim : axis.getDimensions())
        {
            JSON_MAP(writer);
            writer.pair("Name", dim->getName());
            {
                writer.key("Attributes");
                JSON_LIST(writer);
                if(axis.getMeasureDimension() != dim )
                {
                    JSON_MAP(writer);
                    writer.pair("Name", dim->getName());
                    writer.key("Values");
                    {
                        JSON_LIST(writer);
                        const auto& col = axis.getCubeAxis().getDataColumn(dim->getName());
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
                            for(const auto& member : axis.getMeasureDimMembers())
                                writer.value(member.getName());
                        }
                    }
                    {
                        JSON_MAP(writer);
                        writer.pair("Name", "[Measures].[Measures]");
                        writer.pair("IsKey", true);
                        writer.key("Values");
                        {
                            JSON_LIST(writer);
                            for(const auto& member : axis.getMeasureDimMembers())
                                writer.value(member.getName());
                        }
                    }
                }
            }
            idxDim++;
        }
    }

    void writeTuples(JSONWriter& writer, const Axis& axis)
    {
        //std::cout << "**************************************\n";
        //std::cout << "writeTuples\n";
        writer.pair("TupleCount", static_cast<uint32_t>(axis.getTupleCount()));
        //writer.pair("TupleCountTotal", static_cast<uint32_t>(axis.getTupleCount()));
        writer.key("Tuples");
        {
            JSON_LIST(writer);
            for(const auto& dimension : axis.getDimensions())
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
                                if(dimension == axis.getMeasureDimension() )
                                {
                                    size_t maxRows = axis.getCubeAxis().getCardinality();
                                    if(maxRows == 0)
                                        maxRows = 1;
                                    //std::cout << "maxRows: " << maxRows << "\n";
                                    const auto& members = axis.getMeasureDimMembers();
                                    //std::cout << "member.size(): " << member.size() << "\n";
                                    for (size_t rowIndex = 0; rowIndex < maxRows; rowIndex++)
                                    {
                                        for(size_t i = 0; i < members.size(); i++)
                                        {
                                            writer.value(static_cast<uint32_t>(i));
                                        }
                                    }
                                }
                                else
                                {
                                    //std::cout << "axis.getCubeAxis().getCardinality(): " << axis.getCubeAxis().getCardinality() << "\n";
                                    for (size_t rowIndex = 0; rowIndex < axis.getCubeAxis().getCardinality(); rowIndex++)
                                    {
                                        const auto idx = static_cast<uint32_t>(axis.getCubeAxis().getValueIndex(dimension->getName(), rowIndex));
                                        if(axis.getMeasureDimension() != nullptr)
                                        {
                                            const auto& members = axis.getMeasureDimMembers();
                                            for(size_t i = 0; i < members.size(); i++)
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
    }

    void writeCells(const Grid& grid, JSONWriter& writer)
    {
        writer.key("CellArraySizes");
        {
            JSON_LIST(writer);
            writer.value(static_cast<uint32_t>(grid.getCellsSize().first));
            writer.value(static_cast<uint32_t>(grid.getCellsSize().second));
        }
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
                            for(const auto& measure : grid.getCube().getBody())
                            {
                                switch (grid.getCube().getBody().getValueDatatype(measure.getName()))
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
    }
}
