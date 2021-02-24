#pragma once

#include <string>
#include <vector>

#include "grid.h"

namespace query_model
{
	enum {eUninitialize = -1, eDim = 1, eMeas = 2} eQualification;
	typedef std::string Datatype;
	typedef std::string Aggregation;
	typedef std::tuple<const std::string /*name*/, const Datatype, const Aggregation > Object;

	const Aggregation NO_AGG = "";

	class InA_query_model
	{
	public:
		InA_query_model() {};
		
		void setTable(const std::string& table) {m_table = table;};

		void addDim(const std::string& name, const Datatype& datatype);

		void addMeas(const std::string& name, const Datatype& datatype, const Aggregation& agg);

		const std::vector<Object>& getObjects() const { return m_objs;};

		const std::string& getTable() const { return m_table;};

		void prepareGrid(grid::Grid& grid);

	private:
		std::vector<Object> m_objs;
		std::string m_table;
	};
} // query_model