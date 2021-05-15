#include "cube.h"

#include <cstddef>
#include <stdexcept>
#include <numeric>    // For iota
#include <limits>

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

	void Object::materialyze(const Cube& cube)
	{
		m_dataColumn = &*cube.getStorage().getColumn(m_name);
	}

	calculator::eDataType  Object::getDataType() const
	{
		if(!m_dataColumn)
			throw std::runtime_error("Object: materialyze() not called");
		return m_dataColumn->getDataType();
	}

	size_t Object::getRowCount() const
	{
		if(!m_dataColumn)
			throw std::runtime_error("Object: materialyze() not called");
		return m_dataColumn->getRowCount();	
	}

	const Value& Object::getValueAtRowIdx(size_t rowIndex) const
	{
		if(!m_dataColumn)
			throw std::runtime_error("Object: materialyze() not called");
		return m_dataColumn->getValueAtRowIdx(rowIndex);
	}

	size_t Object::getValueIndexFromRowIdx(size_t rowIndex) const
	{
		if(!m_dataColumn)
			throw std::runtime_error("Object: materialyze() not called");
		return m_dataColumn->getValueIndexFromRowIdx(rowIndex);
	}

	const Value& Object::getValueAtValueIdx(size_t valueIndex) const
	{
		if(!m_dataColumn)
			throw std::runtime_error("Object: materialyze() not called");
		return m_dataColumn->getValueAtValueIdx(valueIndex);
	}

	size_t Object::getNumberOfValues() const
	{
		if(!m_dataColumn)
			throw std::runtime_error("Object: materialyze() not called");
		return m_dataColumn->getNumberOfValues();
	}

	
	Value Object::aggregate(const std::vector<size_t>& indexes) const
	{
		Value value;
		if(indexes.empty())
		{
			const calculator::eDataType datatype = m_dataColumn->getDataType();
			if(datatype == eDataType::Number)
				value = std::nan("0");
			else
				value = "##NULL##";
		}
		else if(indexes.size() == 1)
		{
			value = m_dataColumn->getValueAtRowIdx(indexes.at(0));
		}
		else if(indexes.size() > 1 )
		{
			std::cerr << "WASABI: ERROR: Local agregation, NYI hardcoded to sum" << std::endl;

			const calculator::eDataType datatype = m_dataColumn->getDataType();
			if(datatype == eDataType::Number)
			{
				double sum = 0;
				for(const auto& i : indexes)
					sum +=  std::get<double>(m_dataColumn->getValueAtRowIdx(i));
				value = sum;
			}
			else
			{
				value = "#MULTIVALUE";
			}
		}
		return value;
	}

	void Axe::materialyze()
	{
		size_t rowCount = std::numeric_limits<size_t>::max();
		for(auto& object : *this)
		{
			object.materialyze(m_cube);
			if(rowCount == std::numeric_limits<size_t>::max())
				rowCount = object.getRowCount();
			else if(rowCount != object.getRowCount())	 
				throw std::runtime_error("Some column have different row count...");
		}

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
			for(size_t rowIndex = 0; rowIndex < rowCount; rowIndex++)
			{
				tuple.clear();
				for(const auto& obj : *this)
				{
					tuple.push_back(obj.getValueIndexFromRowIdx(rowIndex));
				}
				// Get the list of pre aggreagted indexes
				auto iterUtplesSet = tuplesSet.find(tuple);
				if(iterUtplesSet == tuplesSet.end())
				{
					//std::cout << "+";
					tuplesSet[tuple] = m_tuples.size();
					m_tuples.push_back(std::make_pair(tuple, std::vector(1, rowIndex)));
				}
				else
				{
					//std::cout << "=";
					m_tuples[iterUtplesSet->second].second.push_back(rowIndex);
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

	const Object& Axe::getDimension(const std::string& dimName) const
	{
		for(const auto& dim : (*this))
		{
			if(dim.getName() == dimName)
				return dim;
		}
		throw std::runtime_error("Axe: getDimension() dimemsion not found:" + dimName);
	}

	const Value& Axe::getValue(const std::string& dimName, size_t tupleIndex) const
	{
		for(size_t dimIdx = 0; dimIdx < size(); dimIdx++ )
		{
			if((*this)[dimIdx].getName() == dimName)
				return getValue(dimIdx, tupleIndex);
		}
		throw std::runtime_error("Axe: getValue() dimemsion not found:" + dimName);
	}

	size_t Axe::getValueIndex(const std::string& dimName, size_t tupleIndex) const
	{
		for(size_t dimIdx = 0; dimIdx < size(); dimIdx++ )
		{
			if((*this)[dimIdx].getName() == dimName)
				return getValueIndex(dimIdx, tupleIndex);
		}
		throw std::runtime_error("Axe: getValue() dimemsion not found:" + dimName);
	}

	const Value& Axe::getValue(size_t dimIdx, size_t tupleIndex) const
	{
		return at(dimIdx).getValueAtValueIdx(getValueIndex(dimIdx, tupleIndex));
	}

	size_t Axe::getValueIndex(size_t dimIdx, size_t tupleIndex) const
	{
		if(!m_materialyzed)
			throw std::runtime_error("Axe: materialyze() not called");

		if(tupleIndex >= m_tuples.size())
			throw std::out_of_range("Axe::getValue row");

		const auto& tuple = m_tuples[tupleIndex].first;
		if(dimIdx >= tuple.size())
			throw std::out_of_range("Axe::getValue dimIdx");

		return tuple[dimIdx];
	}

	const std::vector<size_t>& Axe::getParentIndexes(size_t tupleIndex) const
	{
		if(!m_materialyzed)
			throw std::runtime_error("Axe: materialyze() not called");

		if(tupleIndex >= m_tuples.size())
			throw std::out_of_range("Axe::getValue row");

		return m_tuples[tupleIndex].second;
	}

	Body::Body(const Cube& cube, const Axe& row, const Axe& col)
		: m_cube(cube), m_axeRow(row), m_axeCol(col)
	{
	}

	void Body::materialyze()
	{
		//std::cout << "*****************************************\n";
		//std::cout << "materialyze: \n";

		size_t rowCount = std::numeric_limits<size_t>::max();
		for(auto& object : *this)
		{
			object.materialyze(m_cube);
			if(rowCount == std::numeric_limits<size_t>::max())
				rowCount = object.getRowCount();
			else if(rowCount != object.getRowCount())	 
				throw std::runtime_error("Some column have different row count...");
		}

		m_Body.resize(size());
		size_t measIdx = 0;
		for(auto& values : m_Body)
		{
			values.resize(getRowCount());
	
			const Object& measure = at(measIdx++);

			// Full aggreagtion on the 2 axes
			if(m_axeRow.getCardinality() == 0 && m_axeCol.getCardinality() == 0 )
			{
				values.resize(1);
				values[0].resize(1);
				std::vector<size_t> indexes(rowCount);
				std::iota(indexes.begin(), indexes.end(), 0);
				values[0][0] = measure.aggregate(indexes);
			}
			else
			{
				// Full aggreagtion on row axis
				if(m_axeRow.getCardinality() == 0)
				{
					values[0].resize(getColumnCount());
					size_t col = 0;
					for(auto& value : values[0])
					{
						value = measure.aggregate(m_axeCol.getParentIndexes(col++));
					}
				}
				else
				{
					size_t row = 0;
					for(auto& rowValues : values)
					{
						rowValues.resize(getColumnCount());
						std::vector<size_t> rowIdxs = m_axeRow.getParentIndexes(row);

						// Full aggreagtion on col axis
						if(m_axeCol.getCardinality() == 0)
						{
							rowValues[0] = measure.aggregate(rowIdxs);
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
								value = measure.aggregate(indexes);
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

	size_t  Body::getCellCount() const
	{
		return getRowCount() * getColumnCount() * size();
	}

	size_t  Body::getColumnCount() const
	{
		if(empty())
			return 0;
		else if(m_axeCol.empty())
			return m_cube.getStorage().haveData()?1:0;

		return m_axeCol.getCardinality();
	}

	size_t  Body::getRowCount() const
	{
		if(empty())
			return 0;
		else if(m_axeRow.empty())
			return m_cube.getStorage().haveData()?1:0;
		
		return m_axeRow.getCardinality();
	}

	const Object& Body::getMeasure(const std::string& measureName) const
	{
		for(size_t i = 0; i < size(); i++ )
		{
			if(this->at(i).getName() == measureName)
				return this->at(i);
		}
		throw std::runtime_error("Body: getMeasure() measure not found:" + measureName);
	}

	const Value& Body::getValue(const std::string& measureName, size_t col, size_t row) const
	{
		for(size_t i = 0; i < size(); i++ )
		{
			if(this->at(i).getName() == measureName)
				return getValue(i, col, row);
		}
		throw std::runtime_error("Body: getValue() measure not found:" + measureName);
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

		getAxe(axe).push_back(obj);
	}

	void Cube::addMeasure(const Object& obj)
	{
		if(!getStorage().haveCol(obj.getName()))
			throw std::runtime_error("Object " + obj.getName() + " not found in datastorage");

		m_body.push_back(obj);
	}

	void Cube::addFunction(const Object& obj)
	{
		if(getStorage().haveCol(obj.getName()))
			throw std::runtime_error("Function Object " + obj.getName() + " already found in datastorage");

		m_body.push_back(obj);
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
	Axe &Cube::getAxe(eAxe axe)
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
