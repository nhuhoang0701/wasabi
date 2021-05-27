#pragma once

#include <cstddef>
#include <vector>

class indexisSet : public std::vector<size_t>
{
public:
    indexisSet() = default;
    indexisSet(size_t size) : std::vector<size_t>(size) {};
    indexisSet(bool isAll) : std::vector<size_t>(), m_isAll(isAll) {};
    indexisSet& operator=(const std::vector<size_t>& other)
    {
        (std::vector<size_t>&)(*this) = other;

        m_isAll = false;
        return *this;
    }

    bool  isAll() const {return m_isAll;}
    void  setIsAll(bool isAll) {m_isAll = isAll;}

    private:
    bool m_isAll = false;
};
