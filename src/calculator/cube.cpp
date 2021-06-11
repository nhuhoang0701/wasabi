#include "cube.h"

#include "storage.h"

#include <cstdint>
#include <memory>
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
		ScopeDebug sc("Cube::materialyze");
		m_AxeRows.materialyze();
		m_AxeColumns.materialyze();
		m_body.materialyze();

		Logger::debug(" Row axis card", m_AxeRows.getCardinality());
		Logger::debug("  nb dim", m_AxeRows.size());
		Logger::debug(" Col axis card", m_AxeColumns.getCardinality());
		Logger::debug("  nb dim", m_AxeColumns.size());

		Logger::debug(" Body cells nbr", m_body.getCellCount());
		Logger::debug("  nb meas", m_body.getNumberOfVisibleObjects());
		Logger::debug("  nb getColNbrs", m_body.getColumnCount());
		Logger::debug("  nb getRowNbrs", m_body.getRowCount());
	}
	
	const DataStorage&  Cube::getStorage() const
	{
		if(!m_data)
			throw std::runtime_error("Cube storage is missing");
		return *m_data;
	}

	bool Cube::contain(const std::string& name) const
	{
		if( m_body.contain(name) )
			return true;
		for( const auto& dim : m_AxeColumns)
			if(dim->getName() == name)
				return true;
		for( const auto& dim : m_AxeRows)
			if(dim->getName() == name)
				return true;
		
		return false;
	}

	void Cube::addDim(eAxe axe, const Object& obj)
	{
		if(!getStorage().haveCol(obj.getName()))
			throw std::runtime_error("Object " + obj.getName() + " not found in datastorage");

		getAxe(axe).push_back(std::make_shared<Object>(obj.getName()));
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
