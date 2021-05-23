#include "grid.h"

#include <InA_query_model/Query.h>
#include <calculator/cube.h>

#include <cstdint>
#include <json/jsonWriter.h>

#include <iostream>

namespace ina::grid
{
	void    writeAxe(JSONWriter& writer, const Axis& axis);
	void    writeDimensions(JSONWriter& writer, const Axis& axis);
	void    writeTuples(JSONWriter& writer, const Axis& axis);
	void    writeCells(JSONWriter& writer, const Cells& cells);

    void write(const ina::grid::Grid& grid, JSONWriter& writer)
    {
        JSON_MAP(writer);
        {
            writer.pair("ResultFormat", "Version2");
            writer.key("SubSetDescription");
            {
                JSON_MAP(writer);
                writer.pair("ColumnFrom", static_cast<uint32_t>(grid.getColumnFrom()));
                writer.pair("ColumnTo", static_cast<uint32_t>(grid.getColumnTo()));
                writer.pair("RowFrom", static_cast<uint32_t>(grid.getRowFrom()));
                writer.pair("RowTo", static_cast<uint32_t>(grid.getRowTo()));
            }
            writer.key("Axes");
            {
                JSON_LIST(writer);
                writeAxe(writer, grid.m_rowAxe);
                writeAxe(writer, grid.m_colAxe);
            }

            writeCells(writer, grid.getCells());

            writer.pair("ResultSetState", 0u);
            writer.pair("HasErrors", false);
            /*writer.key("Messages");
            {
                JSON_LIST(writer);
                JSON_MAP(writer);
                writer.pair("Number", 1u);
                writer.pair("Text", "This the text of the Error 1 from Grid");
                writer.pair("Type", 2u);
            }*/
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
                size_t idxAttr = 0; // TODO: Remove it use isKey from ina::metadata::Attribut
                {
                    for(const auto& attribut : dim->getAttributes())
                    {
                        bool isKey = idxAttr++==0; // TODO: Remove it use isKey from ina::metadata::Attribut
                        JSON_MAP(writer);
                        writer.pair("Name", attribut.getName());
                        //if(isKey && dim->getAttributes().size()>1 )
                        //    writer.pair("Obtainability", attribut.getObtainability());
                        //writer.pair("IsKey", isKey);
                        writer.key("Values");
                        if(axis.getMeasureDimension() == dim )
                        {
                            JSON_LIST(writer);
                            for(const auto& member : axis.getMeasureDimMembers())
                                writer.value(ina::query_model::Member::getName(member));
                        }
                        else
                        {
                            JSON_LIST(writer);
                            const auto& dimension = axis.getCubeAxis().getDimension(attribut.getName());
                            for(size_t valueIdx = 0; valueIdx < dimension.getNumberOfValues(); valueIdx++)
                            {
                                const auto& data = dimension.getValueAtValueIdx(valueIdx);
                                switch (dimension.getDataType())
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
            idxDim++;
        }
    }

    void writeTuples(JSONWriter& writer, const Axis& axis)
    {
        //std::cout << "**************************************\n";
        //std::cout << "writeTuples\n";
        writer.pair("TupleCount", static_cast<uint32_t>(axis.getTupleCount()));
        writer.pair("TupleCountTotal", static_cast<uint32_t>(axis.getTupleTotalCount()));
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
                                const auto& members = axis.getMeasureDimMembers();
                                const size_t memberDiv = members.size()==0?1:members.size();
                                //std::cout << "member.size(): " << members.size() << "\n";
                                if(dimension == axis.getMeasureDimension() )
                                {
                                    for (size_t tupleIndex = axis.getFrom(); tupleIndex < axis.getTo(); tupleIndex++)
                                    {
                                        writer.value(static_cast<uint32_t>(tupleIndex%memberDiv));
                                    }
                                }
                                else
                                {
                                    //std::cout << "axis.getCubeAxis().getCardinality(): " << axis.getCubeAxis().getCardinality() << "\n";
                                    for (size_t tupleIndex = axis.getFrom(); tupleIndex < axis.getTo(); tupleIndex++)
                                    {
                                        //TODO: dimension->getAttributes().at(0)? Certainly the Key Attributes
                                        // if yes so we should also use ina metadata Dimension here
                                        const auto idx = static_cast<uint32_t>(axis.getCubeAxis().getValueIndex(dimension->getAttributes().at(0).getName(), tupleIndex/memberDiv));
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

    void writeCells(JSONWriter& writer, const Cells& cells)
    {
        writer.key("CellArraySizes");
        {
            JSON_LIST(writer);
            writer.value(static_cast<uint32_t>(cells.getRowCount()));
            writer.value(static_cast<uint32_t>(cells.getColumnCount()));
        }
        writer.key("Cells");
        {
            size_t nbOfMembers = cells.getCubeBody().size();
            JSON_MAP(writer);
            writer.key("Values");
            {
                JSON_MAP(writer);
                writer.pair("Encoding", "None");
                writer.key("Values");
                {
                    JSON_LIST(writer);	
                    for(size_t rowIndex = cells.getRowFrom(); rowIndex < cells.getRowTo(); rowIndex++)
                    {
                        for(size_t colIndex = cells.getColumnFrom(); colIndex < cells.getColumnTo(); colIndex++)
                        {
                            const auto& measure = cells.getCubeBody()[colIndex%nbOfMembers];
                            const auto& data = cells.getCubeBody().getValue(measure.getName(), colIndex/nbOfMembers, rowIndex);
                            switch (measure.getDataType())
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
            //TODO: Not sure why this is needed
            writer.key("ValuesFormatted");
            {
                std::cerr << "WASABI: ERROR: Use harcoded format to display ValuesFormatted." << std::endl;
                JSON_MAP(writer);
                writer.pair("Encoding", "None");
                writer.key("Values");
                {
                    JSON_LIST(writer);	
                    for(size_t rowIndex = cells.getRowFrom(); rowIndex < cells.getRowTo(); rowIndex++)
                    {
                        for(size_t colIndex = cells.getColumnFrom(); colIndex < cells.getColumnTo(); colIndex++)
                        {
                            const auto& measure = cells.getCubeBody()[colIndex%nbOfMembers];
                            const auto& data = cells.getCubeBody().getValue(measure.getName(), colIndex/nbOfMembers, rowIndex);
                            switch (measure.getDataType())
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
            writer.key("CellValueTypes");
            {
                JSON_MAP(writer);
                writer.pair("Encoding", "None");
                writer.key("Values");
                {
                    JSON_LIST(writer);	
                    for(size_t rowIndex = cells.getRowFrom(); rowIndex < cells.getRowTo(); rowIndex++)
                    {
                        for(size_t colIndex = cells.getColumnFrom(); colIndex < cells.getColumnTo(); colIndex++)
                        {
                            const auto& measure = cells.getCubeBody()[colIndex%nbOfMembers];
                            switch (measure.getDataType())
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
