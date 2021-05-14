#include "cube.h"

#include <cstddef>
#include <stdexcept>
#include <numeric>    // For iota

#include <unordered_map>
struct TupleHash {
    size_t operator()(const std::vector<size_t>& v) const {
        //std::hash<size_t> hasher;
        size_t seed = v.size();
        for (size_t i : v) {
            seed ^= i + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};

#include <iostream>


namespace calculator
{
	Object::Object(const std::string& name)
		: m_name(name)
	{
	}

	void Axe::materialyze()
	{
		m_tuples.clear();
		//std::cout << "*****************************************\n";
		//std::cout << "Axe :\n";
		//std::for_each(this->cbegin(), this->cend(), [] (const Object& obj) {std::cout << obj.getName() << "\t";} );
		//std::cout << "\n";
		//std::cout << "Tuples :\n";
		if(empty() == false)
		{
			std::unordered_map<Tuple, size_t, TupleHash> tuplesSet;
			// Get the tuple
			Tuple tuple;
			tuple.reserve(size());
			for(size_t rowIndex = 0; rowIndex <m_cube.getStorage().getRowNbrs(); rowIndex++)
			{
				tuple.clear();
				for(const auto& obj : *this)
				{
					const auto& columnData = m_cube.getStorage().getColumn(obj.getName());
					tuple.push_back(columnData->getValueIndexFromRowIdx(rowIndex));
				}
				// Get the list of pre aggreagted indexes
				if(tuplesSet.find(tuple) == tuplesSet.end())
				{
					//std::cout << "+";
					tuplesSet[tuple] = m_tuples.size();
					m_tuples.push_back(std::make_pair(tuple, std::vector(1, rowIndex)));
				}
				else
				{
					//std::cout << "=";
					m_tuples[tuplesSet.at(tuple)].second.push_back(rowIndex);
				}
				//std::for_each(m_tuples[tuplesSet.at(tuple)].first.cbegin(), m_tuples[tuplesSet.at(tuple)].first.cend(), [] (const size_t c) {std::cout << c << "\t";} );
				//std::cout << "\t|\t";
				//std::for_each(m_tuples[tuplesSet.at(tuple)].second.cbegin(), m_tuples[tuplesSet.at(tuple)].second.cend(), [] (const size_t c) {std::cout << c << "\t";} );
				//std::cout << "\n";
			}
			//std::cout << "\n";
		}
		//std::cout << "End Axe \n";
		m_materialyzed = true;
	}

	size_t Axe::getCardinality() const
	{
		if(!m_materialyzed)
			throw std::runtime_error("Axe: materialyze() not called");
		return m_tuples.size();
	}

	calculator::eDataType Axe::getValueDatatype(const std::string& dimName) const
	{
		for(size_t i = 0; i < size(); i++ )
		{
			if(this->at(i).getName() == dimName)
				return getValueDatatype(i);
		}
		throw std::runtime_error("Axe: getValueDatatype() dimemsion not found");
	}

	const Value& Axe::getValue(const std::string& dimName, size_t row) const
	{
		for(size_t i = 0; i < size(); i++ )
		{
			if(this->at(i).getName() == dimName)
				return getValue(i, row);
		}
		throw std::runtime_error("Axe: getValue() dimemsion not found");
	}

	size_t Axe::getValueIndex(const std::string& dimName, size_t row) const
	{
		for(size_t i = 0; i < size(); i++ )
		{
			if(this->at(i).getName() == dimName)
				return getValueIndex(i, row);
		}
		throw std::runtime_error("Axe: getValue() dimemsion not found");
	}

	calculator::eDataType Axe::getValueDatatype(size_t dimIdx) const
	{
		const std::string& nameCol = at(dimIdx).getName();
		const auto& columnData = m_cube.getStorage().getColumn(nameCol);
		return columnData->getDataType();
	}

	const Value& Axe::getValue(size_t dimIdx, size_t row) const
	{
		const std::string& nameCol = at(dimIdx).getName();
		const auto& columnData = m_cube.getStorage().getColumn(nameCol);
		return columnData->getValueAtValueIdx(getValueIndex(dimIdx, row));
	}

	size_t Axe::getValueIndex(size_t dimIdx, size_t row) const
	{
		if(!m_materialyzed)
			throw std::runtime_error("Axe: materialyze() not called");

		if(row >= m_tuples.size())
			throw std::out_of_range("Axe::getValue row");

		const auto& tuple = m_tuples[row].first;
		if(dimIdx >= tuple.size())
			throw std::out_of_range("Axe::getValue dimIdx");

		const std::string& nameCol = at(dimIdx).getName();
		const auto& columnData = m_cube.getStorage().getColumn(nameCol);
		return tuple[dimIdx];
	}

	const std::vector<size_t>& Axe::getParentIndexes(size_t row) const
	{
		if(!m_materialyzed)
			throw std::runtime_error("Axe: materialyze() not called");

		if(row >= m_tuples.size())
			throw std::out_of_range("Axe::getValue row");

		return m_tuples[row].second;
	}

	const calculator::ColumnData& Axe::getDataColumn(const std::string& dimName) const
	{
		return *m_cube.getStorage().getColumn(dimName);
	}

	Body::Body(const Cube& cube, const Axe& row, const Axe& col)
		: m_cube(cube), m_axeRow(row), m_axeCol(col)
	{
	}

	void agg(Value& value, const ColumnData& columnData, const std::vector<size_t>& indexes)
	{
		if(indexes.empty())
		{
			const calculator::eDataType datatype = columnData.getDataType();
			if(datatype == eDataType::Number)
				value = std::nan("0");
			else
				value = "##NULL##";
		}
		else if(indexes.size() == 1)
		{
			value = columnData.getValueAtRowIdx(indexes.at(0));
		}
		else if(indexes.size() > 1 )
		{
			std::cerr << "WASABI: ERROR: Local agregation, NYI hardcoded to sum" << std::endl;

			const calculator::eDataType datatype = columnData.getDataType();
			if(datatype == eDataType::Number)
			{
				double sum = 0;
				for(const auto& i : indexes)
					sum +=  std::get<double>(columnData.getValueAtRowIdx(i));
				value = sum;
			}
			else
			{
				value = "#MULTIVALUE";
			}
		}
	}

	void Body::materialyze()
	{
		//std::cout << "*****************************************\n";
		//std::cout << "materialyze: \n";

		m_Body.resize(size());
		size_t measIdx = 0;
		for(auto& values : m_Body)
		{
			values.resize(getRowNbrs());
	
			const std::string& measName = at(measIdx++).getName();
			const auto& columnData = *m_cube.getStorage().getColumn(measName);

			// Full aggreagtion on the 2 axes
			if(m_axeRow.getCardinality() == 0 && m_axeCol.getCardinality() == 0 )
			{
				values.resize(1);
				values[0].resize(1);
				std::vector<size_t> indexes(m_cube.getStorage().getRowNbrs());
				std::iota(indexes.begin(), indexes.end(), 0);
				agg(values[0][0], columnData, indexes);
			}
			else
			{
				// Full aggreagtion on row axis
				if(m_axeRow.getCardinality() == 0)
				{
					values[0].resize(getColNbrs());
					size_t col = 0;
					for(auto& value : values[0])
					{
						agg(value, columnData, m_axeCol.getParentIndexes(col++));
					}
				}
				else
				{
					size_t row = 0;
					for(auto& rowValues : values)
					{
						rowValues.resize(getColNbrs());
						std::vector<size_t> rowIdxs = m_axeRow.getParentIndexes(row);

						// Full aggreagtion on col axis
						if(m_axeCol.getCardinality() == 0)
						{
							agg(rowValues[0], columnData, rowIdxs);
						}
						else
						{
							std::sort (rowIdxs.begin(),rowIdxs.end());
							
							size_t col = 0;
							for(auto& value : rowValues)
							{
								std::vector<size_t> indexes;
								const std::vector<size_t>& colParentIdxs = m_axeCol.getParentIndexes(col);
								{
									std::vector<size_t> colIdxs = colParentIdxs;
									std::sort (colIdxs.begin(),colIdxs.end());
										
									indexes.resize(std::min(rowIdxs.size(), colIdxs.size()));
									std::vector<size_t>::iterator it=std::set_intersection (rowIdxs.begin(), rowIdxs.end(), colIdxs.begin(), colIdxs.end(), indexes.begin());
									indexes.resize(it-indexes.begin());
								}
								//std::cout << "[" << row << "]" << "[" << col << "] :" ;
								//std::cout << " rows(";
								//std::for_each(rowIdxs.cbegin(), rowIdxs.cend(), [] (const size_t c) {std::cout << c << ",";} );
								//std::cout << ") cols(";
								//std::for_each(colParentIdxs.cbegin(), colParentIdxs.cend(), [] (const size_t c) {std::cout << c << ",";} );
								//std::cout << ") indexes(";
								//std::for_each(indexes.cbegin(), indexes.cend(), [] (const size_t c) {std::cout << c << ",";} );
								//std::cout << ")" <<  std::endl;

								agg(value, columnData, indexes);
								/*
								if(columnData.getDataType() == eDataType::Number)
									std::cout << "value:" << std::get<double>(values[row][col]) <<  std::endl;
								else
									std::cout << "value:" << std::get<std::string>(values[row][col]) <<  std::endl;
								*/
								col++;
							}
						}
						row++;
					}
				}
			}
		}
		m_materialyzed = true;
	}

	size_t  Body::getCellsNbs() const
	{
		return getRowNbrs() * getColNbrs() * size();
	}

	size_t  Body::getColNbrs() const
	{
		if(empty())
			return 0;
		else if(m_axeCol.empty())
			return m_cube.getStorage().getRowNbrs()==0?0:1;

		return m_axeCol.getCardinality();
	}

	size_t  Body::getRowNbrs() const
	{
		if(empty())
			return 0;
		else if(m_axeRow.empty())
			return m_cube.getStorage().getRowNbrs()==0?0:1;
		
		return m_axeRow.getCardinality();
	}

	calculator::eDataType Body::getValueDatatype(const std::string& measName) const
	{
		for(size_t i = 0; i < size(); i++ )
		{
			if(this->at(i).getName() == measName)
				return getValueDatatype(i);
		}
		throw std::runtime_error("Body: getValueDatatype() measure not found");
	}

	const Value& Body::getValue(const std::string& measName, size_t col, size_t row) const
	{
		for(size_t i = 0; i < size(); i++ )
		{
			if(this->at(i).getName() == measName)
				return getValue(i, col, row);
		}
		throw std::runtime_error("Body: getValue() measure not found");
	}

	calculator::eDataType Body::getValueDatatype(size_t measIdx) const
	{
		const std::string& nameCol = at(measIdx).getName();
		const auto& columnData = m_cube.getStorage().getColumn(nameCol);
		return columnData->getDataType();
	}

	const Value& Body::getValue(size_t measIdx, size_t col, size_t row) const
	{
		if(!m_materialyzed)
			throw std::runtime_error("Body: materialyze() not called");

		return m_Body[measIdx][row][col];
	}

	Cube::Cube()
		: m_AxeRows(*this), m_AxeColumns(*this), m_body(*this, m_AxeRows, m_AxeColumns)
	{
	}
	void Cube::setStorage(std::shared_ptr<const DataStorage> data)
	{
		m_data = data;
	}
	
	void Cube::materialyze()
	{
		m_AxeRows.materialyze();
		m_AxeColumns.materialyze();
		m_body.materialyze();
		/*
		std::cout << "Cube::materialyze()" << std::endl;
		std::cout << " Row axis card:" << m_AxeRows.getCardinality() << std::endl;
		std::cout << "  nb dim:" << m_AxeRows.size() << std::endl;
		std::cout << " Col axis card:" << m_AxeColumns.getCardinality() << std::endl;
		std::cout << "  nb dim:" << m_AxeColumns.size() << std::endl;

		std::cout << " Body cells nbr:" << m_body.getCellsNbs() << std::endl;
		std::cout << "  nb meas:" << m_body.size() << std::endl;
		std::cout << "  nb getColNbrs:" << m_body.getColNbrs() << std::endl;
		std::cout << "  nb getRowNbrs:" << m_body.getRowNbrs() << std::endl;
		*/
	}
	
	const DataStorage&  Cube::getStorage() const
	{
		if(!m_data)
			throw std::runtime_error("Cube storage is missing");
		return *m_data;
	}

	void Cube::addDim(eAxe axe, const Object& obj)
	{
		if(!getStorage().haveCol(obj.getName()))
			throw std::runtime_error("Object " + obj.getName() + " not found in datastorage");

		switch (axe)
		{
		case eAxe::Row:
		{
			m_AxeRows.push_back(obj);
			break;
		}
		case eAxe::Column:
		{
			m_AxeColumns.push_back(obj);
			break;
		}
		default:
		throw std::runtime_error("Unknow eAxe");
		}
	}

	void Cube::addMeas(const std::string& name)
	{
		if(!getStorage().haveCol(name))
			throw std::runtime_error("Object " + name + " not found in datastorage");

		m_body.push_back(name);
	}
	
	const Axe &Cube::getAxe(eAxe axe) const
	{
		switch (axe)
		{
		case eAxe::Row:
			return m_AxeRows;
		case eAxe::Column:
			return m_AxeColumns;
		default:
		throw std::runtime_error("Unknow eAxe");
		}
	}
} // namespace calculator
