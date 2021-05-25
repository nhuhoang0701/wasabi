#pragma once

#include <string_view>
#define KTRACE_API 

#include <iostream>



static size_t depth_log = 0;
inline void indent()
{
    for(size_t i =0; i < depth_log; i++)
        std::cout << '\t';
}

class ScopeLog
{
    public:
    ScopeLog(const std::string_view& name) {indent();std::cout << name << ":" << std::endl;indent();std::cout << '{' << std::endl; depth_log++;}
    ~ScopeLog() {depth_log--;indent();std::cout << '}'  << std::endl;}

    friend void indent();
};