#include "grid.h"

#include <InA_query_model/Query.h>
#include <query_generator/query_generator.h>

#include <calculator/storage.h>
#include <calculator/cube.h>

#include <json/jsonWriter.h>

#include <iostream>
#include <fstream>      // std::ifstream

namespace ina::grid
{
    //TODO: move the following code  to Grid
    void writeGrid(const ina::query_model::Query& query, JSONWriter& writer)
    {
        JSON_MAP(writer);
        const ina::query_model::Dimension* measDim = nullptr; // TODO: Workaround waiting to have a grid
        std::vector<const ina::query_model::Dimension*> rowAxe; // TODO: Workaround waiting to have a grid
        std::vector<const ina::query_model::Dimension*> colAxe; // TODO: Workaround waiting to have a grid
        calculator::Cube cube;
        {
            {
                const std::string& cnxString = query.getDataSource().getPackageName();
                const std::string& tableName = query.getDataSource().getObjectName();
                std::shared_ptr<wasabi::metadata::Catalog> catalog = std::shared_ptr<wasabi::metadata::Catalog>(new wasabi::metadata::Catalog(cnxString));
                const auto& colNames = catalog->getTable(tableName).getColumnNames();
                {
                    writer.key("Object name");
                    JSON_LIST(writer);
                    for(auto& colName : colNames)
                        writer.value(catalog->getTable(tableName).getColumn(colName).getName());
                }
            }
            const query_generator::query_generator& queryGen = query_generator::query_generator(query);

            std::shared_ptr<calculator::DataStorage> data(new calculator::DataStorage());
            queryGen.prepareStorage(*data);
            std::function<void(const dbproxy::Row&)> lambda = [&data](const dbproxy::Row& row)
            {
                data->insertRow(row);
            };
            const std::string& cnxString = query.getDataSource().getPackageName();
            const std::string sql = queryGen.getSQL(*data);
            std::cout << "SQL: " << sql << std::endl;
            dbproxy::DBProxy::getDBProxy(cnxString)->executeSQL(sql, &lambda);
            std::cout << "    Nbrs of rows " << data->getRowNbrs() << std::endl;

            cube.setStorage(data);
            for (const auto& dimension : query.getDefinition().getDimensions()) 
            {
                if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
                    rowAxe.push_back(&dimension);
                else if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
                    colAxe.push_back(&dimension);

                if(!ina::query_model::Dimension::isDimensionOfMeasures(dimension) )
                {
                    if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Rows)
                        cube.addDim(calculator::eAxe::Row, dimension.getName());
                    else if (dimension.getAxe() == ina::query_model::Dimension::eAxe::Columns)
                        cube.addDim(calculator::eAxe::Column, dimension.getName());
                    else throw std::runtime_error("Unknow axis type");
                }
                else
                {
                    measDim = &dimension;
                    for(const auto& member : query.getDefinition().getVisibleMembers(dimension))
                        cube.addMeas(member.getName());
                }
            }
        }
        cube.materialyze();
        {
            writer.pair("ResultFormat", "Version2");
            writer.key("Axes");
            {
                JSON_LIST(writer);
                if(!rowAxe.empty())
                {
                    JSON_MAP(writer);
                    writer.pair("Name", "ROWS");
                    writeDimensions(query, writer, cube, rowAxe);

                    size_t tuplesCount = writeTuples(query, writer, cube, calculator::eAxe::Row, rowAxe);
                    writer.pair("TupleCount", static_cast<uint32_t>(tuplesCount));
                }
                if(!colAxe.empty())
                {
                    JSON_MAP(writer);
                    writer.pair("Name", "COLUMNS");
                    writeDimensions(query, writer, cube, colAxe);

                    size_t tuplesCount = writeTuples(query, writer, cube, calculator::eAxe::Column, colAxe);
                    writer.pair("TupleCount", static_cast<uint32_t>(tuplesCount));
                }
            }

            auto cellsSize = writeCells(query, writer, cube, measDim);
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

    void writeDimensions(const ina::query_model::Query& query, JSONWriter& writer, const calculator::Cube& cube, std::vector<const ina::query_model::Dimension*>& dims)
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
                        const auto& col = *cube.getStorage().getColumn(dim->getName());
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
                            for(const auto& member : query.getDefinition().getVisibleMembers(*dim))
                                writer.value(member.getName() + " (val.[M][N])");
                        }
                    }
                    {
                        JSON_MAP(writer);
                        writer.pair("Name", "[Measures].[Measures]");
                        writer.pair("IsKey", true);
                        writer.key("Values");
                        {
                            JSON_LIST(writer);
                            for(const auto& member : query.getDefinition().getVisibleMembers(*dim))
                                writer.value(member.getName());
                        }
                    }
                }
            }
            idxDim++;
        }
    }

    size_t writeTuples(const ina::query_model::Query& query, JSONWriter& writer, const calculator::Cube& cube, const calculator::eAxe& eAxe, std::vector<const ina::query_model::Dimension*>& dims)
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

        auto const& axis = cube.getAxe(eAxe);
        size_t tuplesCount = axis.getCardinality();
        if(measDim != nullptr)
        {
            if(tuplesCount == 0) // Only dim meas on this axe
                tuplesCount = query.getDefinition().getVisibleMembers(*measDim).size();
            else
                tuplesCount *= query.getDefinition().getVisibleMembers(*measDim).size();
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
                                    const auto& member = query.getDefinition().getVisibleMembers(*dimension);
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
                                            const auto& member = query.getDefinition().getVisibleMembers(*measDim);
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

    std::pair<size_t, size_t> writeCells(const ina::query_model::Query& query, JSONWriter& writer, const calculator::Cube& cube, const ina::query_model::Dimension* measDim)
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
                    for(size_t rowIndex = 0; rowIndex < cube.getBody().getRowNbrs(); rowIndex++)
                    {
                        for(size_t colIndex = 0; colIndex < cube.getBody().getColNbrs(); colIndex++)
                        {
                            for(const auto& measure : cube.getBody())
                            {
                                const auto& data = cube.getBody().getValue(measure.getName(), colIndex, rowIndex);
                                switch (cube.getBody().getValueDatatype(measure.getName()))
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
                    for(size_t rowIndex = 0; rowIndex < cube.getBody().getRowNbrs(); rowIndex++)
                    {
                        for(size_t colIndex = 0; colIndex < cube.getBody().getColNbrs(); colIndex++)
                        {
                            for(const auto& dim : cube.getBody())
                            {
                                const auto& data = cube.getBody().getValue(dim.getName(), colIndex, rowIndex);
                                switch (cube.getBody().getValueDatatype(dim.getName()))
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
                    for(size_t rowIndex = 0; rowIndex < cube.getBody().getRowNbrs(); rowIndex++)
                    {
                        for(size_t colIndex = 0; colIndex < cube.getBody().getColNbrs(); colIndex++)
                        {
                            for(const auto& measName : cube.getBody())
                            {
                                const auto& data = cube.getBody().getValue(measName.getName(), colIndex, rowIndex);
                                switch (cube.getBody().getValueDatatype(measName.getName()))
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
        
        std::pair<size_t, size_t> cellsSize = std::make_pair(cube.getBody().getRowNbrs(), cube.getBody().getColNbrs());
        if(measDim && measDim->getAxe()==ina::query_model::Dimension::eAxe::Rows)
            cellsSize.first *= query.getDefinition().getVisibleMembers(*measDim).size();
        if(measDim && measDim->getAxe()==ina::query_model::Dimension::eAxe::Columns)
            cellsSize.second *= query.getDefinition().getVisibleMembers(*measDim).size();

        return cellsSize;
    }

    Grid::Grid()
    {

    }
}