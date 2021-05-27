#include "cube.h"

#include "storage.h"

#include <cstdint>
#include <stdexcept>
#include <limits>


#include <common/Log.h>


namespace calculator
{
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
		ScopeLog sc("Cube::materialyze");
		m_AxeRows.materialyze();
		m_AxeColumns.materialyze();
		m_body.materialyze();

		Logger::log(" Row axis card", m_AxeRows.getCardinality());
		Logger::log("  nb dim", m_AxeRows.size());
		Logger::log(" Col axis card", m_AxeColumns.getCardinality());
		Logger::log("  nb dim", m_AxeColumns.size());

		Logger::log(" Body cells nbr", m_body.getCellCount());
		Logger::log("  nb meas", m_body.getVisibleObjects().size());
		Logger::log("  nb getColNbrs", m_body.getColumnCount());
		Logger::log("  nb getRowNbrs", m_body.getRowCount());
	}
	
	const DataStorage&  Cube::getStorage() const
	{
		if(!m_data)
			throw std::runtime_error("Cube storage is missing");
		return *m_data;
	}

	bool Cube::contain(const Object& obj) const
	{
		if( m_body.contain(obj) )
			return true;
		if( std::find(m_AxeColumns.begin(), m_AxeColumns.end(), obj) != m_AxeColumns.end() )
			return true;
		if( std::find(m_AxeRows.begin(), m_AxeRows.end(), obj) != m_AxeRows.end() )
			return true;
		
		return false;
	}

	void Cube::addDim(eAxe axe, const Object& obj)
	{
		if(!getStorage().haveCol(obj.getName()))
			throw std::runtime_error("Object " + obj.getName() + " not found in datastorage");

		getAxe(axe).push_back(obj);
	}

	void Cube::addMeasure(const Object& obj)
	{
		m_body.addMeasure(obj);
	}

	void Cube::addFormula(const Object& obj, const ina::query_model::Formula& formula)
	{
		m_body.addFormula(obj, formula);
	}

	void Cube::addRestriction(const Object& obj, const ina::query_model::Selection& selection)
	{
		m_body.addRestriction(obj, selection);
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
