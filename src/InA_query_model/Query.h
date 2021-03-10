#pragma once

#include "DataSource.h"
#include "Definition.h"


#include <memory>
#include <vector>

class JSONGenericObject;    // #include <json/jsonReader.h>

namespace ina::query_model
{
	class Query;

	void read(std::vector<std::shared_ptr<const Query>>& objs, const JSONGenericObject& jsonNode);

	class Query
	{
	public:
		enum eQueryType
		{
		 qUndef,
		 qAnalytics,
		 qMetadata
		};

	private:
		Query(const Query&); // no copy
		Query& operator=(const Query&); // no copy

	public:
		Query() = default;
		~Query();

		bool               haveExpandCube() const;
		eQueryType         getType() const;
		const std::string& getLanguage() const;

		void               setDataSource(const DataSource&);
		const DataSource&  getDataSource() const;
		
		void               setDefinition(const Definition&);
		const Definition&  getDefinition() const;

	private:
		eQueryType      m_type = eQueryType::qUndef;

		std::string     m_language;
		Definition      m_definition;
		DataSource      m_datasource;
		// TODO: should we have more precise semantic ?
		bool            m_isExpandCube = false;

		friend void read(std::shared_ptr<Query>& obj, const JSONGenericObject& jsonNode);
		friend void read(std::vector<std::shared_ptr<const Query>>& objs, const JSONGenericObject& jsonNode);
	};
}
