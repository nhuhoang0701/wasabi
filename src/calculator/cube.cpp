#include "cube.h"

#include <stdexcept>
#include <numeric>    // For iota

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
		if(empty() == false)
		{
			for(size_t row = 0; row <m_cube.m_data->getRowNbrs(); row++)
			{
				// Get the tuple
				std::vector<Value> tuple;
				for(const auto& obj : *this)
				{
					const auto& columnData = m_cube.getStorage().getColumn(obj.getName());
					tuple.push_back((*columnData)[row]);
				}
				// Get the list of pre aggreagted indexes
				if(m_tuples.find(tuple) != m_tuples.end())
					m_tuples[tuple].push_back(row);
				else
					m_tuples[tuple] = std::vector(1, row);
			}
		}
		m_materialyzed = true;
	}

	size_t Axe::getCardinality() const
	{
		if(!m_materialyzed)
			throw std::runtime_error("Axe: materialyze() not called");
		return m_tuples.size();
	}

	calculator::eDataType Axe::getValueDatatype(size_t dimIdx) const
	{
		const std::string& nameCol = at(dimIdx).getName();
		const auto& columnData = m_cube.getStorage().getColumn(nameCol);
		return columnData->getDataType();
	}

	const Value& Axe::getValue(size_t dimIdx, size_t row) const
	{
		if(!m_materialyzed)
			throw std::runtime_error("Axe: materialyze() not called");

		if(row >= m_tuples.size())
			throw std::out_of_range("Axe::getValue row");

		const auto& tuple = std::next(m_tuples.begin(), row)->first;
		if(dimIdx >= tuple.size())
			throw std::out_of_range("Axe::getValue dimIdx");

		return tuple[dimIdx];
	}

	const std::vector<size_t>& Axe::getParentIndexes(size_t row) const
	{
		if(!m_materialyzed)
			throw std::runtime_error("Axe: materialyze() not called");

		if(row >= m_tuples.size())
			throw std::out_of_range("Axe::getValue row");

		return std::next(m_tuples.begin(), row)->second;
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
		else if(indexes.size() > 1 )
		{
			std::cerr << "WASABI: ERROR: Local agregation, NYI hardcoded to sum" << std::endl;

			const calculator::eDataType datatype = columnData.getDataType();
			if(datatype == eDataType::Number)
			{
				double sum = 0;
				for(const auto& i : indexes)
					sum +=  std::get<double>(columnData[i]);
				value = sum;
			}
			else
			{
				value = "#MULTIVALUE";
			}
		}
		else
			value = columnData[indexes.at(0)];
	}

	void Body::materialyze()
	{
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
						std::vector<size_t> rowIdxs = m_axeRow.getParentIndexes(row++);

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
								const std::vector<size_t>& colParentIdxs = m_axeCol.getParentIndexes(col++);
								{
									std::vector<size_t> colIdxs = colParentIdxs;
									std::sort (colIdxs.begin(),colIdxs.end());
										
									indexes.resize(std::min(rowIdxs.size(), colIdxs.size()));
									std::vector<size_t>::iterator it=std::set_intersection (rowIdxs.begin(), rowIdxs.end(), colIdxs.begin(), colIdxs.end(), indexes.begin());
									indexes.resize(it-indexes.begin());
								}

								agg(value, columnData, indexes);
							}
						}
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
