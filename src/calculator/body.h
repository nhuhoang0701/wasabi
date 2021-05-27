#pragma once

#include <common/data.h>

#include "object.h"
#include "axe.h"

#include "indexisSet.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace calculator
{
	class Cube;

	class Body
	{
	public:
		Body(const Cube& cube, const Axe& row,const Axe& col);

		void    materialyze();

		size_t  getCellCount() const;
		size_t  getRowCount() const;
		size_t  getColumnCount() const;

		void    addMeasure(const Object& obj, const ina::query_model::Selection* selection = nullptr);
		void    addFormula(const Object& obj, const ina::query_model::Formula& formula, const ina::query_model::Selection* selection = nullptr);
		void    addRestriction(const Object& obj, const ina::query_model::Selection& formula);

		const std::vector<Object>&  getVisibleObjects() const;
		std::vector<Object>&        getVisibleObjects();

		bool          contain(const Object& obj) const;
		const Object& getObject(const std::string& objectName) const;
		Object&       getObject(const std::string& objectName);

		const common::Value&   getValue(const std::string& measureName, size_t col, size_t row) const;
		const indexisSet&      getParentIndexes(const std::string& measureName, size_t col, size_t row) const;


	private:
		const Cube&  m_cube;
		const Axe&   m_axeRow;
		const Axe&   m_axeCol;

		std::vector<Object>  m_VisibleObjects;

		typedef std::vector<std::vector<std::shared_ptr<const indexisSet>>>  bodyIndexes;
		bodyIndexes  m_BodyIdx;

		typedef std::vector<std::vector<common::Value>>  CellsValue;
		std::map<std::string,CellsValue> m_VisibleCells;
		bool                             m_materialyzed = false;


		
		std::vector<Object>  m_restrictedObjects;
	};
} // cube
