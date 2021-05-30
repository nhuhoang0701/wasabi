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
        indent(std::cout) << '{' << std::endl;
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
        if(value.isDouble())
        {
            indent(std::cout) << "WASABI: '" << name << "' : double(" << value.getDouble() << ")" << std::endl;
        }
        else if(value.isString())
        {
            indent(std::cout) << "WASABI: '" << name << "' : string(" << value.getString() << ")" << std::endl;
        }
        #endif
    }
template<typename M>
    static void log(const M& name, const bool value)
    {
        #if !defined (NDEBUG) && !defined (WASABI_NOLOG)
        if(value)
        {
            indent(std::cout) << "WASABI: '" << name << "' : 'true'" << std::endl;
        }
        else
        {
            indent(std::cout) << "WASABI: '" << name << "' : 'false'" << std::endl;
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
