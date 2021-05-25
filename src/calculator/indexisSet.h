#pragma once

#include <vector>

class indexisSet : public std::vector<size_t>
{
public:
    indexisSet& operator=(const std::vector<size_t>& other)
    {
        (std::vector<size_t>&)(*this) = other;

        return *this;
    }

    bool isAll = false;
};
