#include "cube.h"

#include <stdexcept>

namespace calculator
{
	Object::Object(const std::string& name)
		: m_name(name)
	{
	}
	size_t Axe::getCardinality() const
	{
		size_t card = 0;
		for(const auto& obj : *this)
		{
			size_t colIdx = m_cube.m_data->getColIndex(obj.getName());
			card = std::max(card, std::get<2>((*m_cube.m_data)[colIdx])->getNbDistinctVals());
		}

		return card;
	}

	Body::Body(const Cube& cube, const Axe& row, const Axe& col)
		: m_cube(cube), m_axeRow(row), m_axeCol(col)
	{
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

	calculator::eDataType Body::getValueDatatype(size_t col, size_t row) const
	{
		if(m_cube.getStorage().getRowNbrs() != getRowNbrs())
			throw std::runtime_error("Local agregation, NYI");
		const std::string& nameCol = at(col).getName();
		const auto& column = m_cube.getStorage()[m_cube.getStorage().getColIndex(nameCol)];
		return std::get<0>(column);
	}

	const Value& Body::getValue(size_t col, size_t row) const
	{
		if(m_cube.getStorage().getRowNbrs() != getRowNbrs())
			throw std::runtime_error("Local agregation, NYI");
		const std::string& nameCol = at(col).getName();
		const auto& column = m_cube.getStorage()[m_cube.getStorage().getColIndex(nameCol)];
		const auto& columnData = std::get<2>(column);
		const auto& data = (*columnData)[row];
		return data;
	}

	Cube::Cube()
		: m_AxeRows(*this), m_AxeColumns(*this), m_body(*this, m_AxeRows, m_AxeColumns)
	{
	}
	void Cube::setStorage(std::shared_ptr<const DataStorage> data)
	{
		m_data = data;
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
	
	void materialyze()
	{
	}
} // namespace calculator
