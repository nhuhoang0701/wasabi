
#include "Query.h"
#include "Definition.h"
#include "DataSource.h"

namespace ina::query_model
{
	Query::~Query()
	{
		m_type = qUndef;
	}

	bool Query::haveExpandCube() const
	{
		return m_isExpandCube;
	}

	Query::eQueryType Query::getType() const
	{
		return m_type;
	}

	const std::string& Query::getLanguage() const
	{
		return m_language;
	}

	void Query::setDataSource(const DataSource& ds)
	{
		m_datasource = ds;
	}
	const DataSource& Query::getDataSource() const
	{
		return m_datasource;
	}

	void Query::setDefinition(const Definition& definition)
	{
		m_definition = definition;
	}
	const Definition& Query::getDefinition() const
	{
		return m_definition;
	}
}
