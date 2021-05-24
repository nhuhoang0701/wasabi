#include "DataSource.h"

#include <exceptions/InA_Exception.h>
#include <iostream>

namespace ina::query_model
{
	std::string DataSource::toString(DataSource::Type type)
	{
		switch (type)
		{
			case TypeWasabi:   return "Wasabi";
			case TypeUniverse: return "Universe";
			case TypeCatalogView:  return "CatalogView";
			case TypeView:     return "View";
			case Query:        return "Query";
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
			return TypeCatalogView;
		if ("View" == str)
			return TypeView;
		if("" == str)
			return Query;
		throw TRACED_InA_EXCEPTION(std::string("Unknown datasource type: ") + str);
	}

	DataSource::~DataSource()
	{
		m_type = TypeUndef;
	}

	void DataSource::validate() const
	{
	}

	const std::string&	DataSource::getObjectName() const
	{
		//TODO: Remove just to start dev.
		if(m_objName.empty())
		{
			static std::string defaultObjectName;
			if(defaultObjectName.empty())
			{
				defaultObjectName = "Agg_yr_qt_mt_mn_wk_rg_cy_sn_sr_qt_ma";
				std::cerr << "WASABI: DataSource::getPackageName(): no objectName provided , default will be used: " << defaultObjectName << std::endl;
			}
			return defaultObjectName;
		}
		return m_objName;
	}

	const std::string&	DataSource::getInstanceId() const
	{
		//TODO: Remove just to start dev.
		if(m_InstanceId.empty())
		{
			static std::string defaultInstanceId;
			if(defaultInstanceId.empty())
			{
				defaultInstanceId = "0";
				std::cerr << "WASABI: DataSource::getInstanceId(): no InstanceID provided , default will be used: " << defaultInstanceId << std::endl;
			}
			return defaultInstanceId;
		}
		return m_InstanceId;
	}

	const std::string& DataSource::getPackageName() const
	{
		//TODO: Remove just to start dev.
		if(m_packageName.empty())
		{
			static std::string default_packageName;
			if(default_packageName.empty())
			{
				default_packageName =  "local:sqlite:efashion_lite";
				std::cerr << "WASABI: DataSource::getPackageName(): no package name provided , default will be used: " << default_packageName << std::endl;
			}
			return default_packageName;
		}
		return m_packageName;
	}

	const std::string DataSource::getCatalogType() const
	{
		switch (m_type)
		{
			case TypeUniverse: return "OLAP";
			case TypeView:
			case TypeCatalogView:  return "COLUMN";
			case TypeUndef:
			default: throw TRACED_InA_EXCEPTION("Unknown datasource type");
		}
	}

	const std::string& DataSource::getSchemaName() const
	{
		//TODO: Remove just to start dev.
		if(m_schemaName.empty())
		{
			static std::string defaultSchemaName;
			if(defaultSchemaName.empty())
			{
				defaultSchemaName = "default";
				std::cerr << "WASABI: DataSource::getSchemaName(): no schemaname provided , default will be used: " << defaultSchemaName << std::endl;
			}
			return defaultSchemaName;
		}
		return m_schemaName;
	}

	const std::string&	DataSource::getDescription() const
	{
		//TODO: Remove just to start dev.
		if(m_description.empty())
		{
			static std::string default_description;
			if(default_description.empty())
			{
				default_description =  "Revenue/City/Time";
				std::cerr << "WASABI: DataSource::getDescription(): no description provided , default will be used: " << default_description << std::endl;
			}
			return default_description;
		}
		return m_description;
	}

	int64_t DataSource::getLevelOfDetails() const
	{
		switch (m_type)
		{
			case TypeWasabi: return 13;
			case TypeUniverse: return 5;
			case TypeView:
			case TypeCatalogView:  return 0;
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
