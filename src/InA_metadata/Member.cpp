#include "Member.h"


namespace ina::metadata
{
    Member::Member(const Dimension& dimension,
			const std::pair<std::string, std::string>& uniqueName, 
			const std::pair<std::string, std::string>& name, 
			const std::pair<std::string, std::string>& description)
            : m_uniquename(uniqueName), m_name(name), m_description(description)
    {
    }

    const std::pair<std::string, std::string>& Member::getName() const
    {
        return m_name;
    }

    const std::pair<std::string, std::string>& Member::getUniqueName() const
    {
        return m_uniquename;
    }

    const std::pair<std::string, std::string>& Member::getDescription() const
    {
        return m_description;
    }
}