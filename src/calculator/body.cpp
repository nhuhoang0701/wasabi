//#define WASABI_NOLOG

#include "cube.h"

#include "InA_query_model/QueryEx.h"

#include "storage.h"

#include <memory>
#include <sstream>
#include <stdexcept>
#include <limits>

#include <common/Log.h>


namespace calculator
{

	Body::Body(const Cube& cube, const Axe& row, const Axe& col)
		: m_cube(cube), m_axeRow(row), m_axeCol(col)
	{
	}

	typedef std::tuple<size_t, size_t, const Body*, const ina::query_model::Selection*> Context;
	void getValueCallback(const void* context, const std::string& objName, common::Value& value)
	{
		const Body* body = std::get<2>(*static_cast<const Context*>(context));
		const size_t row = std::get<0>(*static_cast<const Context*>(context));
		const size_t col = std::get<1>(*static_cast<const Context*>(context));

		const size_t indexObj = body->getIndex(objName);
		value = body->getValue(indexObj, col, row);
	}
	void Body::materialyze()
	{
		ScopeLog sc("Body::materialyze()");

		for(auto& object : m_VisibleObjects)
			object.first->materialyze(m_cube);

		if(getRowCount() == 0 || getColumnCount() == 0)
			return;

		////////////////////////////////////////////////////////////
		// Allocation
		for(auto& measure : m_VisibleObjects)
		{
			//Instantiate memory body indexes
			m_BodyIdx.resize(getRowCount());
			for(auto& rowIdx : m_BodyIdx)
			{
				rowIdx.resize(getColumnCount());
			}

			//Create cells entry for this measure
			auto& values = measure.second;
			//Instantiate memory for cells values
			values.resize(getRowCount());
			for(auto& rowValues : values)
			{
				rowValues.resize(getColumnCount());
			}
		}
		
		////////////////////////////////////////////////////////////
		// Compute the parent indexes of each cells
		if(m_axeRow.getCardinality() == 0 && m_axeCol.getCardinality() == 0 )
		{
			auto indexes = std::make_shared<indexisSet>();
			indexes->setIsAll(true);
			m_BodyIdx[0][0] = indexes;
		}
		else
		{
			// Full aggreagtion on row axis
			if(m_axeRow.getCardinality() == 0)
			{
				size_t col = 0;
				for(auto& indexes : m_BodyIdx[0])
				{
					indexes = m_axeCol.getParentIndexes(col++);
				}
			}
			else
			{
				size_t row = 0;
				for(auto& rowIndexes : m_BodyIdx)
				{
					// Full aggreagtion on col axis
					if(m_axeCol.getCardinality() == 0)
					{
						rowIndexes[0] = m_axeRow.getParentIndexes(row);
					}
					else
					{
						std::vector<size_t> rowIdxs = *m_axeRow.getParentIndexes(row);
						std::sort (rowIdxs.begin(),rowIdxs.end());
						
						size_t col = 0;
						for(auto& cellIndexes : rowIndexes)
						{
							const std::vector<size_t>& colParentIdxs = *m_axeCol.getParentIndexes(col++);
							{
								std::vector<size_t> colIdxs = colParentIdxs;
								std::sort (colIdxs.begin(),colIdxs.end());
								
								std::shared_ptr<indexisSet> indexes = std::make_shared<indexisSet>(std::min(rowIdxs.size(), colIdxs.size()));
								std::vector<size_t>::iterator it=std::set_intersection (rowIdxs.begin(), rowIdxs.end(), colIdxs.begin(), colIdxs.end(), indexes->begin());
								indexes->resize(it-indexes->begin());
								cellIndexes = indexes;
							}
							/*			
							std::cout << "[" << row << "]" << "[" << col << "] :" ;
							std::cout << " rows(";
							std::for_each(rowIdxs.cbegin(), rowIdxs.cend(), [] (const size_t c) {std::cout << c << ",";} );
							std::cout << ") cols(";
							std::for_each(colParentIdxs.cbegin(), colParentIdxs.cend(), [] (const size_t c) {std::cout << c << ",";} );
							std::cout << ") indexes(";
							std::for_each(cellIndexes->cbegin(), cellIndexes->cend(), [] (const size_t c) {std::cout << c << ",";} );
							std::cout << ")" <<  std::endl;
							*/
						}
					}
					row++;
				}
			}
		}


		////////////////////////////////////////////////////////////
		// Compute measures
		for(auto& measure : m_VisibleObjects)
		{
			if(measure.first->m_formula != nullptr || measure.first->m_selection != nullptr)
				continue;

			auto& values = measure.second;
			size_t row = 0;
			for(auto& rowValues : values)
			{
				size_t col = 0;
				for(auto& value : rowValues)
				{
					value = measure.first->aggregate(*m_BodyIdx[row][col]);
					col++;
				}
				row++;
			}
		}

		
		////////////////////////////////////////////////////////////
		// Compute formula
		for(auto& measure : m_VisibleObjects)
		{
			if(measure.first->m_formula == nullptr)
				continue;

			auto& values = measure.second;
			size_t row = 0;
			for(auto& rowValues : values)
			{
				size_t col = 0;
				for(auto& value : rowValues)
				{
					Context context(row, col, this, nullptr);
					value = ina::query_model::eval(&context, *measure.first->m_formula, getValueCallback);
					col++;
				}
				row++;
			}
		}
		
		////////////////////////////////////////////////////////////
		// Compute restrictions
		for(auto& restriction : m_VisibleObjects)
		{
			if(restriction.first->m_selection == nullptr)
				continue;

			auto& values = restriction.second;
			size_t row = 0;
			for(auto& rowValues : values)
			{
				size_t col = 0;
				for(auto& value : rowValues)
				{
					value = restriction.first->m_restrictedObject->aggregate(*m_BodyIdx[row][col], restriction.first->m_selection);
					col++;
				}
				row++;
			}
		}

		m_materialyzed = true;
	}

	size_t  Body::getCellCount() const
	{
		return getRowCount() * getColumnCount() * getNumberOfVisibleObjects();
	}

	size_t  Body::getColumnCount() const
	{
		if(m_VisibleObjects.empty())
			return 0;
		else if(m_axeCol.empty())
			return m_cube.getStorage().haveData()?1:0;

		return m_axeCol.getCardinality();
	}

	size_t  Body::getRowCount() const
	{
		if(m_VisibleObjects.empty())
			return 0;
		else if(m_axeRow.empty())
			return m_cube.getStorage().haveData()?1:0;
		
		return m_axeRow.getCardinality();
	}

	size_t Body::getNumberOfVisibleObjects() const
	{
		return m_VisibleObjects.size();
	}
	const Object& Body::getVisibleObject(size_t index) const
	{
		return *m_VisibleObjects.at(index).first;
	}

	bool Body::contain(const std::string& name) const
	{
		for( const auto& object : m_VisibleObjects )
			if(object.first->getName() == name)
				return true;
		
		return false;
	}

	void Body::addMeasure(const Object& obj, const ina::query_model::Selection* selection)
	{
		ScopeLog sc("Body::addMeasure");
		Logger::log("name",  obj.getName());
		Logger::log("selection", (selection!=nullptr));
		if(contain(obj.getName()))
			throw std::runtime_error("Object name already use");
		if(selection==nullptr)
			m_VisibleObjects.push_back(std::make_pair(std::make_shared<Object>(obj.getName()), CellsValue()));
		else
		 	m_restrictedObjects.push_back(std::make_shared<Object>(obj.getName()));
	}

	const Object& Body::getObject(const std::string& measureName) const
	{
		return *const_cast<Body&>(*this)._getObject(measureName);
	}

	std::shared_ptr<Object>& Body::_getObject(const std::string& measureName)
	{
		for(auto& obj :m_VisibleObjects )
		{
			if(obj.first->getName() == measureName)
				return obj.first;
		}
		throw std::runtime_error("Body: getMeasure(): measure not found:" + measureName);
	}

	void Body::addFormula(const Object& obj, const ina::query_model::Formula& formula, const ina::query_model::Selection* selection)
	{
		ScopeLog sc("Body::addFormula");
		Logger::log("name",  obj.getName());
		Logger::log("selection", (selection!=nullptr));
		std::vector<std::string> deps;
		ina::query_model::getDeps(formula, deps);
		for(const auto& dep : deps)
		{
			if(contain(dep) == false)
				addMeasure(dep, selection);
		}

		m_VisibleObjects.push_back(std::make_pair(std::make_shared<Object>(obj.getName()), CellsValue()));
		m_VisibleObjects.back().first->m_formula = &formula;
	}

	void Body::addRestriction(const Object& obj, const ina::query_model::Selection& selection)
	{
		ScopeLog sc("addRestriction");
		Logger::log("name",  obj.getName());
		std::vector<std::string> deps;
		ina::query_model::getDeps(selection, deps);
		Logger::log("deps[0]",  deps[0]);
		if(contain(deps[0]) == false)
			addMeasure(deps[0]);

		m_VisibleObjects.push_back(std::make_pair(std::make_shared<Object>(obj.getName()), CellsValue()));
		m_VisibleObjects.back().first->m_restrictedObject = _getObject(deps[0]);
		m_VisibleObjects.back().first->m_selection = &selection;
	}

	size_t Body::getIndex(const std::string& objName) const
	{
		for(size_t index = 0; index < m_VisibleObjects.size(); index++)
			if(m_VisibleObjects[index].first->getName() == objName)
				return index;

		throw std::runtime_error("Body: getIndex(): object not found:" + objName);
	}

	const common::Value& Body::getValue(size_t index, size_t col, size_t row) const
	{
		return m_VisibleObjects[index].second[row][col];
	}
} // namespace calculator
