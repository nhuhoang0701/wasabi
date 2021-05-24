#pragma once

#include "InA_query_model/Selection/SelectionElement.h"
#include "calculator/storage.h"
#include <string>

namespace calculator {class DataStorage;}
namespace ina::query_model {class QueryEx;}
namespace ina::metadata {class Cube;}

namespace query_generator
{
	class query_generator
	{
	public:
		query_generator(const ina::query_model::QueryEx& queryExec);

		void prepareStorage(std::shared_ptr<calculator::DataStorage> data) const;

		std::string getSQL() const;

	private:
		mutable std::shared_ptr<const calculator::DataStorage> m_dataStorage;
		const ina::query_model::QueryEx& m_queryExec;
		void buildWhereClause(const ina::query_model::SelectionElement& selectionOperator, std::ostringstream& where) const;
		void buildWhereSetOperandClause(const ina::query_model::SelectionElement& selectionElement, ina::query_model::LogicalOperator parentLogicalOperator, std::ostringstream& where) const;
	};
} // query_generator
