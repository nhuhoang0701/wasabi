#include "cube.h"

#include <stdexcept>


namespace calculator
{
	Object::Object(const std::string& name)
		: m_name(name)
	{
	}

	void Axe::materialyze()
	{
		for(size_t row = 0; row <m_cube.m_data->getRowNbrs(); row++)
		{
			std::vector<Value> tuple;
			for(const auto& obj : *this)
			{
				const auto& columnData = (*m_cube.m_data)[m_cube.getStorage().getColIndex(obj.getName())];
				const auto& data = (*columnData)[row];

				tuple.push_back(data);
			}
			if(!tuple.empty())
				m_tuples.insert(tuple);
		}
		m_materialyzed = true;
	}

	size_t Axe::getCardinality() const
	{
		if(!m_materialyzed)
			throw std::runtime_error("Axe: materialyze() not called");
		return m_tuples.size();
	}

	calculator::eDataType Axe::getValueDatatype(size_t colIdx) const
	{
		const std::string& nameCol = at(colIdx).getName();
		const auto& columnData = m_cube.getStorage()[m_cube.getStorage().getColIndex(nameCol)];
		return columnData->getDataType();
	}

	const Value& Axe::getValue(size_t colIdx, size_t row) const
	{
		if(row >= m_tuples.size())
			throw std::out_of_range("Axe::getValue col");

		const auto& tuple = *std::next(m_tuples.begin(), row);

		if(colIdx >= tuple.size())
			throw std::out_of_range("Axe::getValue col");

		return tuple[colIdx];
	}


	Body::Body(const Cube& cube, const Axe& row, const Axe& col)
		: m_cube(cube), m_axeRow(row), m_axeCol(col)
	{
	}

	void Body::materialyze()
	{
		// TODO
	}

	size_t  Body::getCellsNbs() const
	{
		return getRowNbrs() * getColNbrs();
	}

	size_t  Body::getColNbrs() const
	{
		return std::vector<Object>::size();
	}

	size_t  Body::getRowNbrs() const
	{
		if(empty())
			return 0;
		else if(m_axeRow.empty() && m_axeCol.empty())
			return m_cube.getStorage().getRowNbrs()==0?0:1;
		else if(!m_axeRow.empty() && !m_axeCol.empty())
			return m_axeRow.getCardinality() * m_axeCol.getCardinality();
		else if(!m_axeRow.empty() && m_axeCol.empty())
			return m_axeRow.getCardinality();
		else if(m_axeRow.empty() && !m_axeCol.empty())
			return m_axeCol.getCardinality();

		return 0; // TODO: Error
	}

	calculator::eDataType Body::getValueDatatype(size_t colIdx) const
	{
		if(m_cube.getStorage().getRowNbrs() != getRowNbrs())
			throw std::runtime_error("Local agregation, NYI");
		const std::string& nameCol = at(colIdx).getName();
		const auto& columnData = m_cube.getStorage()[m_cube.getStorage().getColIndex(nameCol)];
		return columnData->getDataType();
	}

	const Value& Body::getValue(size_t colIdx, size_t row) const
	{
		if(m_cube.getStorage().getRowNbrs() != getRowNbrs())
			throw std::runtime_error("Local agregation, NYI");
		const std::string& nameCol = at(colIdx).getName();
		const auto& columnData = m_cube.getStorage()[m_cube.getStorage().getColIndex(nameCol)];
		return (*columnData)[row];
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
