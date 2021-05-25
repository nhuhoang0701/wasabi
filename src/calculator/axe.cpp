#include "axe.h"

#include <stdexcept>

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
				for(const auto& dimension : *this)
				{
					tuple.push_back(dimension.getValueIndexFromRowIdx(rowIndex));
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

	const common::Value& Axe::getValue(const std::string& dimName, size_t tupleIndex) const
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

	const common::Value& Axe::getValue(size_t dimIdx, size_t tupleIndex) const
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

} // namespace calculator
