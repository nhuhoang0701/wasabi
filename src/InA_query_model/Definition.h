#pragma once

#include <metadata_enrichment/Tables.h>

#include "DataSource.h"
#include "InA_query_model/QuerySort.h"
#include "InA_query_model/Selection/Selection.h"
#include "InA_query_model/Selection/Element.h"
#include "Member.h"
#include "Dimension.h"
#include "QuerySort.h"
#include "Formula.h"
#include "ResultSetFeatureRequest.h"

#include <string>
#include <vector>
#include <ostream>

class JSONGenericObject;    // #include <json/jsonReader.h>

namespace ina::query_model
{
	using namespace wasabi;

	typedef std::string Datatype;
	typedef std::string Aggregation;

	const Aggregation NO_AGG = "";

	class Definition;
	void read(Definition& obj, const JSONGenericObject& jsonNode);
	void buildQueryFilter(Definition& obj, const JSONGenericObject& jsonNode);

	class Definition
	{
	public:
		Definition() = default;
		
		void                          addDimension(const Dimension & dimension);
		const std::vector<Dimension>& getDimensions() const;

		//TODO: Should not be here, workaround due to the missing queryspec like or Grid object
		const std::vector<Member>    getVisibleMembers(const Dimension& dimension) const;

		const Selection & 			 getSelection() const;
		void 			 setSelection(Selection & selection);

		const std::vector<QuerySort>& getQuerySorts() const;
		void                          addQuerySort(const QuerySort& querySort);

		const ResultSetFeatureRequest& getResultSetFeat() const;

	private:
		Selection m_selection;

		std::vector<Dimension>             m_dimensions;
		std::vector<Element> m_filters;
		std::vector<QuerySort>             m_sorts;
		ResultSetFeatureRequest            m_resultSetFeature;
		
		DataSource m_ds;

		static void traverse(std::vector<Element>& filters, const SelectionElement& selectionElement);
		
		friend void read(Definition & obj, const JSONGenericObject& jsonNode);
	};

} // query_model
