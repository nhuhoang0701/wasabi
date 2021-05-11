#include "Member.h"


namespace ina::metadata
{
    Member::Member(const Dimension& dimension, const std::string& uniqueName, const std::string& name, const std::string& description)
    {
        std::string suffixName;
        #ifdef DEBUG
        std::string suffixName = " (N.)";
        #endif
        std::string suffixDesc;
        #ifdef DEBUG
        std::string suffixDesc = " (D.)";
        #endif
        std::string suffixUniqueNAme;
        #ifdef DEBUG
        std::string suffixUniqueNAme = " (U.N.)";
        #endif
        m_uniquename = {"[Measures].[Measures]", uniqueName+suffixUniqueNAme};
        m_name = {"[Measures].[Name]", name+suffixName};
        m_description ={"[Measures].[Description]", description+suffixDesc};
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