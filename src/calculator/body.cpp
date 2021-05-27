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
	void getValueCallback(const void* context, const std::string& name, common::Value& value)
	{
		const Body* body = std::get<2>(*static_cast<const Context*>(context));
		const size_t row = std::get<0>(*static_cast<const Context*>(context));
		const size_t col = std::get<1>(*static_cast<const Context*>(context));

		value = body->getValue(name, col, row);
	}
	void Body::materialyze()
	{
		ScopeLog sc("Body::materialyze()");

		for(auto& object : m_VisibleObjects)
			object.materialyze(m_cube);

		if(getRowCount() == 0 || getColumnCount() == 0)
			return;

		////////////////////////////////////////////////////////////
		// Allocation
		for(const auto& measure : m_VisibleObjects)
		{
			//Instantiate memory body indexes
			m_BodyIdx.resize(getRowCount());
			for(auto& rowIdx : m_BodyIdx)
			{
				rowIdx.resize(getColumnCount());
			}

			//Create cells entry for this measure
			auto& values = m_VisibleCells.insert(std::pair(measure.getName(), CellsValue())).first->second;
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
		for(const auto& measure : m_VisibleObjects)
		{
			if(measure.m_formula != nullptr || measure.m_selection != nullptr)
				continue;

			auto& values = m_VisibleCells.at(measure.getName());
			size_t row = 0;
			for(auto& rowValues : values)
			{
				size_t col = 0;
				for(auto& value : rowValues)
				{
					value = measure.aggregate(*m_BodyIdx[row][col]);
					col++;
				}
				row++;
			}
		}

		
		////////////////////////////////////////////////////////////
		// Compute formula
		for(const auto& measure : m_VisibleObjects)
		{
			if(measure.m_formula == nullptr)
				continue;

			auto& values = m_VisibleCells.at(measure.getName());
			size_t row = 0;
			for(auto& rowValues : values)
			{
				size_t col = 0;
				for(auto& value : rowValues)
				{
					Context context(row, col, this, nullptr);
					value = ina::query_model::eval(&context, *measure.m_formula, getValueCallback);
					col++;
				}
				row++;
			}
		}
		
		////////////////////////////////////////////////////////////
		// Compute restrictions
		for(const auto& restriction : m_VisibleObjects)
		{
			if(restriction.m_selection == nullptr)
				continue;

			auto& values = m_VisibleCells.at(restriction.getName());
			size_t row = 0;
			for(auto& rowValues : values)
			{
				size_t col = 0;
				for(auto& value : rowValues)
				{
					value = restriction.m_restrictedObject->aggregate(*m_BodyIdx[row][col], restriction.m_selection);
					col++;
				}
				row++;
			}
		}

		m_materialyzed = true;
	}

	size_t  Body::getCellCount() const
	{
		return getRowCount() * getColumnCount() * getVisibleObjects().size();
	}

	size_t  Body::getColumnCount() const
	{
		if(getVisibleObjects().empty())
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

	const std::vector<Object>& Body::getVisibleObjects() const
	{
		return m_VisibleObjects;
	}
	std::vector<Object>& Body::getVisibleObjects()
	{
		return m_VisibleObjects;
	}

	bool Body::contain(const Object& obj) const
	{
		for( const auto& object : m_VisibleObjects )
			if(object.getName() == obj.getName())
				return true;
		
		return false;
	}

	void Body::addMeasure(const Object& obj, const ina::query_model::Selection* selection)
	{
		ScopeLog sc("addMeasure");
		Logger::log("name",  obj.getName());
		Logger::log("selection", (selection!=nullptr));
		if(selection==nullptr)
			m_VisibleObjects.push_back(obj);
		else
		 	m_restrictedObjects.push_back(obj);
	}

	const Object& Body::getObject(const std::string& measureName) const
	{
		return const_cast<Body&>(*this).getObject(measureName);
	}

	Object& Body::getObject(const std::string& measureName)
	{
		for(size_t i = 0; i < getVisibleObjects().size(); i++ )
		{
			if(m_VisibleObjects.at(i).getName() == measureName)
				return getVisibleObjects()[i];
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
				addMeasure(dep);
		}

		m_VisibleObjects.push_back(obj);
		m_VisibleObjects.back().m_formula = &formula;
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

		m_VisibleObjects.push_back(obj);
		m_VisibleObjects.back().m_restrictedObject = &getObject(deps[0]);
		m_VisibleObjects.back().m_selection = &selection;
	}

	const common::Value& Body::getValue(const std::string& measureName, size_t col, size_t row) const
	{
		return m_VisibleCells.at(measureName)[row][col];
	}
	const indexisSet& Body::getParentIndexes(const std::string& measureName, size_t col, size_t row) const
	{
		return *m_BodyIdx[row][col];
	}

} // namespace calculator
