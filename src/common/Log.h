#pragma once

#include "common/data.h"
#include <sstream>
#include <string_view>
#define KTRACE_API 

#include <iostream>



static size_t depth_log = 0;
inline std::ostream& indent(std::ostream& out)
{
    for(size_t i =0; i < depth_log; i++)
    {
        out << "\t ";
    }
    return out;
}

class ScopeLog
{
    public:
    ScopeLog(const std::string_view& name)
    {
        #if !defined (NDEBUG) && !defined (WASABI_NOLOG)
        indent(std::cout) << name << ":" << std::endl;
        indent(std::cout)<< '{' << std::endl;
        depth_log+=1;
        #endif
    }
    ~ScopeLog()
    {
        #if !defined (NDEBUG) && !defined (WASABI_NOLOG)
        depth_log-=1;
        indent(std::cout) << '}'  << std::endl;
        #endif
    }
};

class Logger
{
    public:
template<typename T> 
    static void error(const T& error_message)
    {
        std::stringstream sstr;
        sstr << "WASABI error:'" << error_message << "'" << std::endl;
        indent(std::cerr) << sstr.str();
    }
template<typename M, typename V> 
    static void error(const M& error_message, const V& error_value)
    {
        std::stringstream sstr;
        sstr  << "WASABI error: '" << error_message << "'" << ":'" << error_value << "'" << std::endl;
        indent(std::cerr) << sstr.str();
    }
template<typename M, typename V>
    static void log(const M& name, const V& value)
    {
        #if !defined (NDEBUG) && !defined (WASABI_NOLOG)
        indent(std::cout) << "WASABI: '" << name << "' : '" << value << "'" << std::endl;
        #endif
    }
template<typename M>
    static void log(const M& name, const common::Value& value)
    {
        #if !defined (NDEBUG) && !defined (WASABI_NOLOG)
        if(std::holds_alternative<double>(value))
        {
            indent(std::cout) << "WASABI: '" << name << "' : '" << std::get<double>(value) << "'" << std::endl;
        }
        else if(std::holds_alternative<std::string>(value))
        {
            indent(std::cout) << "WASABI: '" << name << "' : '" << std::get<std::string>(value) << "'" << std::endl;
        }
        #endif
    }
template<typename T>
    static void debug(std::string_view name, const T& value)
    {
        #if !defined (NDEBUG) && !defined (WASABI_NOLOG)
        indent(std::cout) << "WASABI: '" << name << "' : '" << value << "'" << std::endl;
        #endif
    }
};
