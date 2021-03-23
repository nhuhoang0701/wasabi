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

	Body::Body(const Axe& row, const Axe& col)
		: m_axeRow(row), m_axeCol(col)
	{
	}

	size_t  Body::getCellsNbs() const
	{
		if(empty())
			return 0;
		else if(m_axeRow.empty() && m_axeCol.empty())
			return size();
		else if(!m_axeRow.empty() && !m_axeCol.empty())
			return size() * m_axeRow.getCardinality() * m_axeCol.getCardinality();
		else if(!m_axeRow.empty() && m_axeCol.empty())
			return size() * m_axeRow.getCardinality();
		else if(m_axeRow.empty() && !m_axeCol.empty())
			return size() * m_axeCol.getCardinality();

		return 0; // TODO: Error
	}

	Cube::Cube()
		: m_AxeRows(*this), m_AxeColumns(*this), m_body(m_AxeRows, m_AxeColumns)
	{
	}
	void Cube::setStorage(std::shared_ptr<const DataStorage> data)
	{
		m_data = data;
	}

	void Cube::addDim(eAxe axe, const Object& obj)
	{
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

		if(!m_data->haveCol(obj.getName()))
			throw std::runtime_error("Object " + obj.getName() + " not present in data");
	}

	void Cube::addMeas(const std::string& name)
	{
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
