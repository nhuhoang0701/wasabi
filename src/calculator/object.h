#pragma once

#include "indexisSet.h"

#include <common/data.h>

#include <memory>
#include <string>

namespace ina::query_model {class Formula; class Selection;}

namespace calculator
{
	class Cube;
	class ColumnData;
	class DataStorage;

	// Create subclass for Formaul, Restriction...
	class Object
	{
	public:
		Object(const std::string& name);

		bool operator==(const Object& rhs) const;

		void materialyze(const Cube& cube);

		const std::string& getName() const {return m_name;}

		common::eDataType  getDataType() const;

		size_t                 getRowCount() const;
		const common::Value&   getValueAtRowIdx(size_t rowIndex) const;

		size_t                 getValueIndexFromRowIdx(size_t rowIndex) const;
		const common::Value&   getValueAtValueIdx(size_t valueIndex) const;
		size_t                 getNumberOfValues() const;

		common::Value          aggregate(const indexisSet& rowIndexes, const ina::query_model::Selection* selection = nullptr) const;

		const ina::query_model::Formula* m_formula = nullptr; // TMP: WIP
	
		std::shared_ptr<Object>            m_restrictedObject; // TMP: WIP
		const ina::query_model::Selection* m_selection = nullptr; // TMP: WIP
		const calculator::DataStorage*     m_datastorage = nullptr; // TMP: WIP
	private:
		std::string  m_name;

		std::shared_ptr<const ColumnData> m_dataColumn;
		
		common::Value          aggregate(const ina::query_model::Selection* selection = nullptr) const;
	};

	class ObjectFormula
	{
	public:
		ObjectFormula(const std::string& name);

		ObjectFormula& operator=(const ObjectFormula& rhs);

		bool operator==(const ObjectFormula& rhs) const;

		void materialyze(const Cube& cube);

		common::eDataType  getDataType() const;

		size_t                 getRowCount() const;
		const common::Value&   getValueAtRowIdx(size_t rowIndex) const;

		size_t                 getValueIndexFromRowIdx(size_t rowIndex) const;
		const common::Value&   getValueAtValueIdx(size_t valueIndex) const;
		size_t                 getNumberOfValues() const;

		common::Value          aggregate(const indexisSet& rowIndexes, const ina::query_model::Selection* selection = nullptr) const;

		const ina::query_model::Formula* m_formula = nullptr; // TMP: WIP
	private:
		common::Value          aggregate(const ina::query_model::Selection* selection = nullptr) const;
	};

} // cube
