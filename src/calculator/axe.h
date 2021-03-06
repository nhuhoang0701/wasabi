#pragma once

#include "object.h"

#include <common/data.h>

#include <memory>
#include <vector>
#include <string>
#include <tuple>

namespace calculator
{
	class Cube;

	class Axe : public std::vector<std::shared_ptr<Object>>
	{
	public:
		Axe(const Cube& cube) : m_cube(cube) {};
		~Axe() = default;

		void   materialyze();

		size_t getCardinality() const;

		const Object&          getDimension(const std::string& dimName) const;

		const common::Value&   getValue(const std::string& dimName, size_t tupleIndex) const;
		size_t                 getValueIndex(const std::string& dimName, size_t tupleIndex) const;

	private:
		const common::Value&   getValue(size_t dimIdx, size_t tupleIndex) const;
		size_t                 getValueIndex(size_t dimIdx, size_t tupleIndex) const;

	private:
		friend class Body;
		std::shared_ptr<const indexisSet>   getParentIndexes(size_t tupleIndex) const;
	
	private:
		const Cube&                  m_cube;

		// Tuple of value index
		typedef std::vector<size_t> Tuple;
		/*Tuple / row indexes in the parent data table for this tuple, will be use for the aggreagtion*/
		std::vector<std::pair<Tuple, std::shared_ptr<indexisSet>>> m_tuples;
		bool  m_materialyzed = false;
	};

} // cube
