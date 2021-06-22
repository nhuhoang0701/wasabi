#include "grid.h"

#include <InA_query_model/Dimension.h>
#include <calculator/cube.h>

#include <cstdint>
#include <json/jsonWriter.h>

#include <common/Log.h>

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
                    /**/
                    bool haveAlways=false;
                    for(const auto& attribut : dim->getAttributes())
                        if(attribut.getObtainability()=="Always")
                            haveAlways = true;
                    /**/
                    for(const auto& attribut : dim->getAttributes())
                    {
                        JSON_MAP(writer);
                        writer.pair("Name", attribut.getName());
                        //TODO: writer.pair("IsKey", attribut.isKey());
                       /**/
                        bool isKey = idxAttr==0; // TODO: Remove it use isKey from ina::metadata::Attribut
                        idxAttr++;
                        std::string Obtainability = attribut.getObtainability();
                        if(Obtainability!="Always")
                        {
                            if(isKey && dim->getAttributes().size()>1 )
                            {
                                if(attribut.getObtainability()!="UserInterface")
                                {
                                    Logger::error("Receive Obtainability!=UserInterface on key attribut", attribut.getName());
                                }
                                Obtainability = "UserInterface";
                            }
                            else if(haveAlways==false)
                            {
                                if(attribut.getObtainability()!="Always")
                                {
                                    Logger::error("Receive Obtainability!=Always on text attribut: ", attribut.getName());
                                }
                                Obtainability = "Always";
                                haveAlways = true;
                            }
                        }
                        writer.pair("Obtainability", Obtainability);
                        /**/
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
                                case common::eDataType::String:
                                {
                                    writer.value(data.getString());
                                    break;
                                }
                                case common::eDataType::Numeric:
                                {
                                    writer.value(data.getDouble());
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
                                const size_t memberDiv = members.empty()?1:members.size();
                                //std::cout << "member.size(): " << members.size() << "\n";
                                for (size_t tupleIndex = axis.getFrom(); tupleIndex < axis.getTo(); tupleIndex++)
                                {
                                    uint32_t idx = -1;
                                    if(dimension == axis.getMeasureDimension() )
                                    {
                                        idx = (tupleIndex%memberDiv);
                                    }
                                    else
                                    {
                                        //TODO: dimension->getAttributes().at(0)? Certainly the Key Attributes
                                        // if yes so we should also use ina metadata Dimension here
                                        idx = static_cast<uint32_t>(axis.getCubeAxis().getValueIndex(dimension->getAttributes().at(0).getName(), tupleIndex/memberDiv));
                                    }
                                    writer.value(idx);
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
            size_t memberDivCol = cells.getColumnAxis().getMeasureDimMembers().size();
            if(memberDivCol==0)
                memberDivCol=1;
            size_t memberDivRow = cells.getRowAxis().getMeasureDimMembers().size();
            if(memberDivRow==0)
                memberDivRow=1;
            bool dimMeasureOnCol = cells.getColumnAxis().getMeasureDimension()!=nullptr;
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
                            const size_t indexInCube = dimMeasureOnCol?(colIndex%memberDivCol):(rowIndex%memberDivRow);
                            const auto& measure = cells.getCubeBody().getVisibleObject(indexInCube);
                            const auto& data = cells.getCubeBody().getValue(indexInCube, colIndex/memberDivCol, rowIndex/memberDivRow);
                            switch (measure.getDataType())
                            {
                            case common::eDataType::String:
                            {
                                writer.value(data.getString());
                                break;
                            }
                            case common::eDataType::Numeric:
                            {
                                writer.value(data.getDouble());
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
                Logger::error("Use harcoded format to display ValuesFormatted.");
                JSON_MAP(writer);
                writer.pair("Encoding", "None");
                writer.key("Values");
                {
                    JSON_LIST(writer);	
                    for(size_t rowIndex = cells.getRowFrom(); rowIndex < cells.getRowTo(); rowIndex++)
                    {
                        for(size_t colIndex = cells.getColumnFrom(); colIndex < cells.getColumnTo(); colIndex++)
                        {
                            const size_t indexInCube = dimMeasureOnCol?(colIndex%memberDivCol):(rowIndex%memberDivRow);
                            const auto& measure = cells.getCubeBody().getVisibleObject(indexInCube);
                            const auto& data = cells.getCubeBody().getValue(indexInCube, colIndex/memberDivCol, rowIndex/memberDivRow);
                            switch (measure.getDataType())
                            {
                            case common::eDataType::String:
                            {
                                writer.value(data.getString());
                                break;
                            }
                            case common::eDataType::Numeric:
                            {
                                writer.value(data.getDouble());
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
                            const auto& measure = cells.getCubeBody().getVisibleObject(dimMeasureOnCol?(colIndex%memberDivCol):(rowIndex%memberDivRow));
                            switch (measure.getDataType())
                            {
                            case common::eDataType::String:
                            {
                                writer.value(4u);
                                break;
                            }
                            case common::eDataType::Numeric:
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
