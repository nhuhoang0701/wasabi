#pragma once

#include "storage.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <set>

namespace calculator
{
	class Cube;
	enum class eAxe {Row, Column};
	class Object
	{
	public:
		Object() = default;
		Object(const std::string& name);

		bool operator==(const Object& rhs) const;

		void materialyze(const Cube& cube);

		const std::string& getName() const {return m_name;}

		calculator::eDataType  getDataType() const;

		size_t                 getRowCount() const;
		const Value&           getValueAtRowIdx(size_t rowIndex) const;

		size_t                 getValueIndexFromRowIdx(size_t rowIndex) const;
		const Value&           getValueAtValueIdx(size_t valueIndex) const;
		size_t                 getNumberOfValues() const;

		Value                  aggregate() const;
		Value                  aggregate(const std::vector<size_t>& rowIndexes) const;

		bool m_isConstant = false; // TMP: WIP
		Value m_constant = 0.0;  // TMP: WIP
	private:
		std::string  m_name;

		std::shared_ptr<const ColumnData> m_dataColumn = nullptr;
	};

	class Column;
	class Body;
	class Axe : public std::vector<Object>
	{
	public:
		Axe(const Cube& cube) : m_cube(cube) {};
		~Axe() = default;

		void   materialyze();

		size_t getCardinality() const;

		const Object&          getDimension(const std::string& dimName) const;

		const Value&           getValue(const std::string& dimName, size_t tupleIndex) const;
		size_t                 getValueIndex(const std::string& dimName, size_t tupleIndex) const;

	private:
		const Value&           getValue(size_t dimIdx, size_t tupleIndex) const;
		size_t                 getValueIndex(size_t dimIdx, size_t tupleIndex) const;

	private:
		friend class Body;
		const std::vector<size_t>&     getParentIndexes(size_t tupleIndex) const;
	
	private:
		const Cube&                  m_cube;

		// Tuple of value index
		typedef std::vector<size_t> Tuple;
		/*Tuple / row indexes in the parent data table for this tuple, will be use for the aggreagtion*/
		std::vector<std::pair<Tuple, std::vector<size_t>>> m_tuples;
		bool  m_materialyzed = false;
	};

	class Body : public std::vector<Object>
	{
	public:
		Body(const Cube& cube, const Axe& row,const Axe& col);

		void    materialyze();

		size_t  getCellCount() const;
		size_t  getRowCount() const;
		size_t  getColumnCount() const;

		const Object&          getMeasure(const std::string& measureName) const;
		const Value&           getValue(const std::string& measureName, size_t col, size_t row) const;

	private:
		const Value&           getValue(size_t measIdx, size_t col, size_t row) const;

	private:
		const Cube&  m_cube;
		const Axe&   m_axeRow;
		const Axe&   m_axeCol;

		typedef std::vector<std::vector<Value>>  CellsValue;

		std::vector<CellsValue> m_Body;
		bool  m_materialyzed = false;
	};
	class Cube
	{
	public:
		Cube();

		void                setStorage(std::shared_ptr<const DataStorage> data);
		const DataStorage&  getStorage() const;

		bool         contain(const Object& obj) const;
		
		void         addDim(eAxe eAxe, const Object& obj);
		void         addMeasure(const Object& obj);
		void         addConstant(const Object& obj, const Value& value);

		void         materialyze();

		Axe&         getAxe(eAxe axe);
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
