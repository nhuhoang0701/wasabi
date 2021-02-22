#pragma once

#include <string>
#include <vector>

namespace query_model
{
	enum {eUninitialize = -1, eDim = 1, eMeas = 2} eQualification;
	typedef std::string Datatype;
	typedef std::string Aggregation;
	typedef std::tuple<const std::string& /*name*/, const Datatype&, const Aggregation& > Object;

	class Grid;

	class InA_query_model
	{
	public:
		InA_query_model() {};
		
		void setTable(const std::string& table) {m_table = table;};
		void addDim(const std::string& name, const Datatype& datatype);
		void addMeas(const std::string& name, const Datatype& datatype, const Aggregation& agg);

		const std::vector<Object>& getObjects() const { return m_objs;};

		const std::string& getTable() const { return m_table;};

		void prepareGrid(Grid& grid);

	private:
		std::vector<Object> m_objs;
		std::string m_table;
	};
} // query_model
