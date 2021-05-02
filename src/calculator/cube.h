#pragma once

#include "storage.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <set>

namespace calculator
{
	enum class eAxe {Row, Column};
	class Object
	{
	public:
		Object() = default;
		Object(const std::string& name);

		const std::string& getName() const {return m_name;}

	private:
		std::string m_name;
	};

	class Cube;
	class Body;
	class Axe : public std::vector<Object>
	{
	public:
		Axe(const Cube& cube) : m_cube(cube) {};
		~Axe() = default;

		void   materialyze();

		size_t getCardinality() const;

		calculator::eDataType  getValueDatatype(const std::string& dimName) const;
		const Value&           getValue(const std::string& dimName, size_t row) const;
		size_t                 getValueIndex(const std::string& dimName, size_t row) const;

	private:
		calculator::eDataType  getValueDatatype(size_t dimIdx) const;
		const Value&           getValue(size_t dimIdx, size_t row) const;
		size_t                 getValueIndex(size_t dimIdx, size_t row) const;

	private:
		friend class Body;
		const std::vector<size_t>&     getParentIndexes(size_t row) const;
	
	private:
		const Cube&                  m_cube;

		// Tuple of value index
		typedef std::vector<size_t> Tuple;
		std::map<Tuple, std::vector<size_t> /*row index tp aggregated*/> m_tuples;
		bool  m_materialyzed = false;
	};

	class Body : public std::vector<Object>
	{
	public:
		Body(const Cube& cube, const Axe& row,const Axe& col);

		void    materialyze();

		size_t  getCellsNbs() const;
		size_t  getRowNbrs() const;
		size_t  getColNbrs() const;

		calculator::eDataType  getValueDatatype(const std::string& measName) const;
		const Value&           getValue(const std::string& measName, size_t col, size_t row) const;

	private:
		calculator::eDataType  getValueDatatype(size_t measIdx) const;
		const Value&           getValue(size_t measIdx, size_t col, size_t row) const;

	private:
		const Cube& m_cube;
		const Axe& m_axeRow;
		const Axe& m_axeCol;

		typedef std::vector<std::vector<Value>>  Values;

		std::vector<Values> m_Body;
		bool  m_materialyzed = false;
	};
	class Cube
	{
	public:
		Cube();

		void                setStorage(std::shared_ptr<const DataStorage> data);
		const DataStorage&  getStorage() const;
		
		void         addDim(eAxe eAxe, const Object& obj);
		void         addMeas(const std::string& name);

		void         materialyze();

		const Axe&   getAxe(eAxe eAxe) const;
		const Body&  getBody() const {return m_body;};

	private:
		friend class Axe;
		Axe  m_AxeRows;
		Axe  m_AxeColumns;

		std::shared_ptr<const DataStorage>  m_data;

		Body   m_body;
	};
} // cube
