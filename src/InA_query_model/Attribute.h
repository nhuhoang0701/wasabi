#pragma once

#include <string>
#include <vector>
#include <map>

namespace ina::query_model
{
	class Dimension;

	enum eFilterable {IsFilterable, IsNotFilterable};
	enum eDisplayed {IsDisplayed, IsNotDisplayed};

	enum class ePresentationType
	{
		Undefined = -1,

		Key,
		Text,

		HierarchyKey,
		HierarchyText,
		HierarchyDisplayKey
	};
	std::string toString(const ePresentationType type);

	class Attribute
	{
	public:

		enum class eType
		{
			Undefined = -1,
			// For flat mode - always exist
			Key,
			Description,
			Name,

			// Hierarchical mode - only for hierarchical dims
			HKey,
			HPath,
			HName,
			HDescription,

			// Common
			Generic
		};
		static std::string toString(eType attType);

		Attribute(    eType attType,
					  const Dimension& dim,
					  const std::string& name,
					  const std::string& description,
					  const std::string& alias,
				      //const InA_Types::DataType& dataType,
					  eFilterable flt, eDisplayed displ);

		virtual ~Attribute();

		eType                        getAttributeType() const;
		virtual const std::string&   getName() const;
		virtual const std::string&   getDescription() const;
		//virtual Qualification        getQualification() const;
		//virtual InA_Types::DataType  getDataType() const;
		const std::string&           getAlias() const;

		//virtual InA_Types::StoryAggregation getStoryAggregation() const   {return InA_Types::stAggregUndef;}

		const std::string            getDataTypeAsString() const;
		int                          getDataTypeAsInt() const;

		const Dimension&             getDimension() const;

		ePresentationType            getPresentationType() const;

		bool                   isFilterable() const;
		bool                   isKey() const;
		bool                   isFlatKey() const;
		bool                   isPath() const;
		bool                   isDisplayed() const;

	private:
		std::string                 m_name;
		//InA_Types::DataType         m_dataType;
		eType                       m_attType = eType::Undefined;
		std::string                 m_description;
		std::string                 m_alias;
		ePresentationType           m_presentationType = ePresentationType::Undefined;
		bool                        m_isFilterable;
		bool                        m_isDisplayed;
		const Dimension*            m_dim = nullptr;
	};

} // namespace
