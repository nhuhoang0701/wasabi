#pragma once


#include <memory>
#include <vector>

class JSONGenericObject;    // #include <json/jsonReader.h>

namespace ina::query_model
{
	class Queries;

	void read(Queries& objs, const JSONGenericObject& jsonNode);

	class Query;
	class Queries : public std::vector<std::shared_ptr<const Query>>
	{
	public:
		Queries() = default;

		bool    isBatch() const;

	private:
		bool            m_isBatch = false;

		friend void read(Queries& objs, const JSONGenericObject& jsonNode);
	};
}
