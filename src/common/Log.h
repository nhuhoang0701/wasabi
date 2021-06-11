
#pragma once

#define WASABI_LOG
//#define WASABI_DEBUG
#define WASABI_ERROR

#if defined (WASABI_LOG) || defined (WASABI_DEBUG) || defined (WASABI_ERROR)
#define WASABI_SOMELOG_ENABLE
#endif

#if defined (WASABI_SOMELOG_ENABLE)
#define WASABI_LOGGER_PARAM(parameter) parameter
#else
#define WASABI_LOGGER_PARAMLOG(parameter)
#endif

#if defined (WASABI_LOG)
#define WASABI_LOGGER_PARAMLOG(parameter) parameter
#else
#define WASABI_LOGGER_PARAMLOG(parameter)
#endif

#if defined (WASABI_ERROR)
#define WASABI_LOGGER_PARAMERROR(parameter) parameter
#else
#define WASABI_LOGGER_PARAMERROR(parameter)
#endif

#if defined (WASABI_DEBUG)
#define WASABI_LOGGER_PARAMDEBUG(parameter) parameter
#else
#define WASABI_LOGGER_PARAMDEBUG(parameter)
#endif

#include "common/data.h"
#include <sstream>
#include <string_view>
#define KTRACE_API 

#include <iostream>



static size_t depth_log = 0;
inline std::ostream& indent(std::ostream& out)
{
    #if defined (WASABI_SOMELOG_ENABLE)
    for(size_t i =0; i < depth_log; i++)
    {
        out << "\t ";
    }
    #endif
    return out;
}

class ScopeLog
{
    public:
    inline ScopeLog(const std::string_view& WASABI_LOGGER_PARAM(name))
    {
        #if defined (WASABI_SOMELOG_ENABLE)
        indent(std::cout) << name << ":" << std::endl;
        indent(std::cout) << '{' << std::endl;
        depth_log+=1;
        #endif
    }
    inline ~ScopeLog()
    {
        #if defined (WASABI_SOMELOG_ENABLE)
        depth_log-=1;
        indent(std::cout) << '}'  << std::endl;
        #endif
    }
};

class ScopeDebug
{
    public:
    inline ScopeDebug(const std::string_view& WASABI_LOGGER_PARAMDEBUG(name))
    {
        #if defined (WASABI_DEBUG)
        indent(std::cout) << name << ":" << std::endl;
        indent(std::cout) << '{' << std::endl;
        depth_log+=1;
        #endif
    }
    inline ~ScopeDebug()
    {
        #if defined (WASABI_DEBUG)
        depth_log-=1;
        indent(std::cout) << '}'  << std::endl;
        #endif
    }
};

class Logger
{
    public:
template<typename T> 
    static void error(const T& WASABI_LOGGER_PARAMERROR(error_message))
    {
        #if defined (WASABI_ERROR)
        std::stringstream sstr;
        sstr << "WASABI error:'" << error_message << "'" << std::endl;
        indent(std::cerr) << sstr.str();
        #endif
    }
template<typename M, typename V> 
    static void error(const M& WASABI_LOGGER_PARAMERROR(error_message), const V& WASABI_LOGGER_PARAMERROR(error_value))
    {
        #if defined (WASABI_ERROR)
        std::stringstream sstr;
        sstr  << "WASABI error: '" << error_message << "'" << ":'" << error_value << "'" << std::endl;
        indent(std::cerr) << sstr.str();
        #endif
    }
template<typename M, typename V>
    static void log(const M& WASABI_LOGGER_PARAMLOG(name), const V& WASABI_LOGGER_PARAMLOG(value))
    {
        #if defined (WASABI_LOG)
        indent(std::cout) << "WASABI: '" << name << "' : '" << value << "'" << std::endl;
        #endif
    }
template<typename M>
    static void log(const M& WASABI_LOGGER_PARAMLOG(name), const common::Value& WASABI_LOGGER_PARAMLOG(value))
    {
        #if defined (WASABI_LOG)
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
    static void log(const M& WASABI_LOGGER_PARAMLOG(name), const bool WASABI_LOGGER_PARAMLOG(value))
    {
        #if defined (WASABI_LOG)
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
    static void debug(std::string_view WASABI_LOGGER_PARAMDEBUG(name), const T& WASABI_LOGGER_PARAMDEBUG(value))
    {
        #if !defined (NDEBUG) && defined (WASABI_DEBUG)
        indent(std::cout) << "WASABI: '" << name << "' : '" << value << "'" << std::endl;
        #endif
    }
    static void debug(std::string_view WASABI_LOGGER_PARAMDEBUG(name))
    {
        #if !defined (NDEBUG) && defined (WASABI_DEBUG)
        indent(std::cout) << "WASABI: '" << name << "'" << std::endl;
        #endif
    }
};
