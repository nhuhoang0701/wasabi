#include "cube.h"

#include "InA_query_model/QueryEx.h"

#include "storage.h"

#include <stdexcept>
#include <limits>


#include <iostream>


namespace calculator
{

	Body::Body(const Cube& cube, const Axe& row, const Axe& col)
		: m_cube(cube), m_axeRow(row), m_axeCol(col)
	{
	}

	typedef std::tuple<size_t, size_t, const Body*> Context;
	void getValueCallback(const void* context, const std::string& name, common::Value& value)
	{
		const Body* body = std::get<2>(*static_cast<const Context*>(context));
		const size_t row = std::get<0>(*static_cast<const Context*>(context));
		const size_t col = std::get<1>(*static_cast<const Context*>(context));
		value = body->getValue(name, col, row);
	}
	void Body::materialyze()
	{
		//std::cout << "*****************************************\n";
		//std::cout << "materialyze: \n";

		for(auto& object : *this)
		{
			object.materialyze(m_cube);
		}

		if(getRowCount() == 0 || getColumnCount() == 0)
			return;
			
		for(const auto& measure : *this)
		{
			//Instantiate memory body indexes
			m_BodyIdx.resize(getRowCount());
			for(auto& rowIdx : m_BodyIdx)
			{
				rowIdx.resize(getColumnCount());
			}

			//Create cells entry for this measure
			auto& values = m_Cells.insert(std::pair(measure.getName(), CellsValue())).first->second;
			//Instantiate memory for cells values
			values.resize(getRowCount());
			for(auto& rowValues : values)
			{
				rowValues.resize(getColumnCount());
			}
		}
		
		if(m_axeRow.getCardinality() == 0 && m_axeCol.getCardinality() == 0 )
		{
			m_BodyIdx[0][0].isAll = true;
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
					//rowIndexes.resize(getColumnCount());

					// Full aggreagtion on col axis
					if(m_axeCol.getCardinality() == 0)
					{
						rowIndexes[0] = m_axeRow.getParentIndexes(row);
					}
					else
					{
						std::vector<size_t> rowIdxs = m_axeRow.getParentIndexes(row);
						std::sort (rowIdxs.begin(),rowIdxs.end());
						
						size_t col = 0;
						for(auto& value : rowIndexes)
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
							value = indexes;
							col++;
						}
					}
					row++;
				}
			}
		}


		size_t measIdx = 0;
		for(const auto& measure : *this)
		{
			auto& values = m_Cells.at(measure.getName());
			if(measure.m_formula != nullptr || measure.m_selection != nullptr)
				continue;

			size_t row = 0;
			for(auto& rowValues : values)
			{
				size_t col = 0;
				for(auto& value : rowValues)
				{
					value = measure.aggregate(m_BodyIdx[row][col]);
					col++;
				}
				row++;
			}
		}

		
		for(const auto& measure : *this)
		{
			if(measure.m_formula == nullptr)
				continue;

			auto& values = m_Cells.at(measure.getName());
			size_t row = 0;
			for(auto& rowValues : values)
			{
				size_t col = 0;
				for(auto& value : rowValues)
				{
					Context context(row, col, this);
					value = ina::query_model::eval(&context, *measure.m_formula, getValueCallback);
					col++;
				}
				row++;
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

	void Body::addMeasure(const Object& obj)
	{
		//std::cout << "addMeasure:" << obj.getName() << std::endl;
		push_back(obj);
	}

	bool Body::contain(const Object& obj) const
	{
		if( std::find(cbegin(), cend(), obj) != end() )
			return true;
		
		return false;
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

	void Body::addFormula(const Object& obj, const ina::query_model::Formula& formula)
	{
		//std::cout << "addFormula:" << obj.getName() << std::endl;
		std::vector<std::string> deps;
		ina::query_model::getDeps(formula, deps);
		for(const auto& dep : deps)
		{
			if(contain(dep) == false)
				addMeasure(dep);
		}

		push_back(obj);
		back().m_formula = &formula;
	}

	void Body::addRestriction(const Object& obj, const ina::query_model::Selection& selection)
	{
		//std::cout << "addRestriction:" << obj.getName() << std::endl;
		std::vector<std::string> deps;
		ina::query_model::getDeps(selection, deps);
		for(const auto& dep : deps)
		{
			if(contain(dep) == false)
				addMeasure(dep);
		}

		push_back(obj);
		back().m_selection = &selection;
	}

	const common::Value& Body::getValue(const std::string& measureName, size_t col, size_t row) const
	{
		return m_Cells.at(measureName)[row][col];
	}

} // namespace calculator
