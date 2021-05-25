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

	class Object
	{
	public:
		Object() = default;
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

		common::Value          aggregate(const indexisSet& rowIndexes) const;

		const ina::query_model::Formula* m_formula = nullptr; // TMP: WIP
		const ina::query_model::Selection* m_selection = nullptr; // TMP: WIP
	private:
		std::string  m_name;

		std::shared_ptr<const ColumnData> m_dataColumn = nullptr;
		
		common::Value          aggregate() const;
	};

} // cube
