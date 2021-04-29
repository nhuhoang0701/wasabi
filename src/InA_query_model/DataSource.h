#pragma once

#include <string>
#include <vector>
#include <memory>

class JSONGenericObject; // From <json/jsonReader.h>
class JSONWriter;        // From <json/jsonWriter.h>

namespace ina::query_model
{
	class DataSource;

	void read(DataSource & obj, const JSONGenericObject& jsonNode);
	void write(const DataSource& obj, JSONWriter& jsonNode);

	class DataSource
	{
	public:
		enum Type
		{
			TypeUndef = -1,
			TypeUniverse,
			TypeWasabi,
			TypeCatalogView,
			TypeView,
			Query
		};
		static std::string toString(Type type);
		static Type        fromString(const std::string& str);

	private:
		DataSource(const DataSource&);
	public:
		DataSource() = default;
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
		Type            m_type = TypeUndef;
		std::string		m_packageName;
		std::string     m_schemaName;
		std::string		m_description;
	};
}
