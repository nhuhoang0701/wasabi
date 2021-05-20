#pragma once
#include "InA_query_model/Selection/SelectionElement.h"
#include "calculator/storage.h"
#include <string>

namespace calculator {class DataStorage;}
namespace ina::query_model {class Query;}

namespace query_generator
{
	class query_generator
	{
	public:
		query_generator(const ina::query_model::Query& query) : m_query(query) {};

		void prepareStorage(calculator::DataStorage& cube) const;

		std::string getSQL(const calculator::DataStorage& data) const;

	private:
		const ina::query_model::Query& m_query;
		void buildWhereClause(const ina::query_model::SelectionElement& selectionOperator, std::ostringstream& where) const;
	};
} // query_generator
