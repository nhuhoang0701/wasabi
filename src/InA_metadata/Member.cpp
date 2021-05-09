#include "Member.h"


namespace ina::metadata
{
    Member::Member(const Dimension& dimension, const std::string& uniqueName, const std::string& name, const std::string& description)
    {
        m_uniquename = {"[Measures].[Measures]", uniqueName};
        m_name = {"[Measures].[Name]", name};
        m_description ={"[Measures].[Description]", description};
    }

    Member::Member(const Member& other)
    {
        m_uniquename = other.m_uniquename;
        m_name = other.m_name;
        m_description = other.m_description;
    }
    Member& Member::operator=(const Member& other)
    {
        m_uniquename = other.m_uniquename;
        m_name = other.m_name;
        m_description = other.m_description;

        return *this;
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