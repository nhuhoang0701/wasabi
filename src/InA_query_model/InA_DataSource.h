#pragma once

#include <string>
#include <vector>
#include <memory>

class JSONGenericObject; // From <json/jsonReader.h>
class JSONWriter;        // From <json/jsonWriter.h>

namespace InA_DataSource
{
	class DataSource;

	void read(DataSource & ds, const JSONGenericObject& dataSource);
	void write(const DataSource& ds, JSONWriter& writer);

	class DataSource
	{
	public:
		enum Type
		{
			TypeUndef = -1,
			TypeUniverse,
			TypeWasabi,
			TypeCatalog
		};
		static std::string toString(Type type);
		static Type        fromString(const std::string& str);

	private:
		DataSource(const DataSource&);
	public:
		DataSource();
		~DataSource();

		void  validate() const;

		const std::string&	getObjectName() const;
		void                setObjectName(const std::string& name);

		const std::string&  getInstanceId() const;
		void                setInstanceId(const std::string& uuid);

		Type                getType() const;
		void                setType(const Type& type);

		const std::string&  getPackageName() const;
		void                setPackageName(const std::string& catalog);

		const std::string   getCatalogType() const;

		const std::string&  getSchemaName() const;
		void                setSchemaName(const std::string& schema);

		const std::string&  getDescription() const;
		void                setDescription(const std::string& description);

		int64_t             getLevelOfDetails() const;

		const std::string   getHash() const;

	private:
		std::string		m_objName;
		std::string		m_InstanceId;
		Type            m_type;
		std::string		m_packageName;
		std::string     m_schemaName;
		std::string		m_description;
	};
}
