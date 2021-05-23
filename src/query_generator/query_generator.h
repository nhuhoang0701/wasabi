#pragma once
#include "InA_query_model/Selection/SelectionElement.h"
#include "calculator/storage.h"
#include <string>

namespace calculator {class DataStorage;}
namespace ina::query_model {class Query;}
namespace ina::metadata {class Cube;}

namespace query_generator
{
	class query_generator
	{
	public:
		query_generator(const ina::query_model::Query& query);

		void prepareStorage(calculator::DataStorage& cube, const ina::metadata::Cube* dsCube) const;

		std::string getSQL(const calculator::DataStorage& data, const ina::metadata::Cube* dsCube) const;

	private:
		const ina::query_model::Query& m_query;
		void buildWhereClause(const ina::query_model::SelectionElement& selectionOperator, std::ostringstream& where) const;
		void buildWhereSetOperandClause(const ina::query_model::SelectionElement& selectionElement, ina::query_model::LogicalOperator parentLogicalOperator, std::ostringstream& where) const;
	};
} // query_generator
