#include "InA_DataSource.h"

#include <exceptions/InA_Exception.h>

namespace ina::query_model
{
	std::string DataSource::toString(DataSource::Type type)
	{
		switch (type)
		{
			case TypeWasabi:   return "Wasabi";
			case TypeUniverse: return "Universe";
			case TypeCatalog:  return "CatalogView";
			case TypeUndef:    return "Undefined";
			default: throw TRACED_InA_EXCEPTION("Unknown datasource type");
		}
	}

	DataSource::Type DataSource::fromString(const std::string& str)
	{
		if ("Wasabi" == str)
			return TypeWasabi;
		if ("Universe" == str)
			return TypeUniverse;
		if ("Unx" == str)
			return TypeUniverse;
		if ("CatalogView" == str)
			return TypeCatalog;
		throw TRACED_InA_EXCEPTION(std::string("Unknown datasource type: ") + str);
	}

	DataSource::DataSource()
		: m_type(TypeUndef)
	{
	}

	DataSource::~DataSource()
	{
	}

	void DataSource::validate() const
	{
	}

	const std::string&	DataSource::getObjectName() const
	{
		return m_objName;
	}

	const std::string&	DataSource::getInstanceId() const
	{
		return m_InstanceId;
	}

	const std::string& DataSource::getPackageName() const
	{
		return m_packageName;
	}

	const std::string DataSource::getCatalogType() const
	{
		switch (m_type)
		{
			case TypeUniverse: return "OLAP";
			case TypeCatalog:  return "COLUMN";
			case TypeUndef:
			default: throw TRACED_InA_EXCEPTION("Unknown datasource type");
		}
	}

	const std::string& DataSource::getSchemaName() const
	{
		return m_schemaName;
	}

	const std::string&	DataSource::getDescription() const
	{
		return m_description;
	}

	int64_t DataSource::getLevelOfDetails() const
	{
		switch (m_type)
		{
			case TypeWasabi: return 13;
			case TypeUniverse: return 5;
			case TypeCatalog:  return 0;
			case TypeUndef:
			default: throw TRACED_InA_EXCEPTION("Unknown datasource type");
		}
	}

	void DataSource::setObjectName(const std::string& name)
	{
		m_objName = name;
	}

	void DataSource::setInstanceId(const std::string& instanceId)
	{
		m_InstanceId = instanceId;
	}

	DataSource::Type DataSource::getType() const
	{
		return m_type;
	}

	void DataSource::setType(const DataSource::Type& type)
	{
		m_type = type;
	}

	void DataSource::setPackageName(const std::string& catalog)
	{
		m_packageName = catalog;
	}

	void DataSource::setSchemaName(const std::string& schema)
	{
		m_schemaName = schema;
	}

	void DataSource::setDescription(const std::string& description)
	{
		m_description = description;
	}

	const std::string DataSource::getHash() const
	{
		throw TRACED_InA_EXCEPTION("NYI");
	}
}
