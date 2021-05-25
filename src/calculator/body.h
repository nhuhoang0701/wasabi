#pragma once

#include <common/data.h>

#include "object.h"
#include "axe.h"

#include "indexisSet.h"

#include <string>
#include <vector>
#include <map>

namespace calculator
{
	class Cube;

	class Body : public std::vector<Object>
	{
	public:
		Body(const Cube& cube, const Axe& row,const Axe& col);

		void    materialyze();

		size_t  getCellCount() const;
		size_t  getRowCount() const;
		size_t  getColumnCount() const;

		void    addMeasure(const Object& obj);
		void    addFormula(const Object& obj, const ina::query_model::Formula& formula);
		void    addRestriction(const Object& obj, const ina::query_model::Selection& formula);

		bool          contain(const Object& obj) const;
		const Object& getMeasure(const std::string& measureName) const;

		const common::Value&   getValue(const std::string& measureName, size_t col, size_t row) const;


	private:
		const Cube&  m_cube;
		const Axe&   m_axeRow;
		const Axe&   m_axeCol;


		typedef std::vector<std::vector<indexisSet>>  bodyIndexes;
		bodyIndexes  m_BodyIdx;

		typedef std::vector<std::vector<common::Value>>  CellsValue;
		std::map<std::string,CellsValue> m_Cells;
		bool                             m_materialyzed = false;
	};
} // cube
