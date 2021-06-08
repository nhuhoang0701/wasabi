#include "InA_Exception.h"

#include <common/Log.h>

#include <exception>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <typeinfo>


// see: https://code.woboq.org/llvm/libcxxabi/src/cxa_exception.cpp.html

#if defined (__cplusplus)
extern "C"
{
#endif

void *__cxa_allocate_exception(size_t thrown_size) throw()
{
	void* ptr = malloc(thrown_size);
	if(ptr == nullptr)
		std::terminate();
	//TODO: Need a logger to log error but not in the console
	return ptr;
}

//This string is hardcoded in WASI js side
static const std::string KEYWORD_TO_THROW_Ecception = "throw this message";
void __cxa_throw(void *thrown_object, std::type_info *tinfo, void (*dest)(void *))
{
	std::string typeinfo =  "unknown";
	std::string msg =  KEYWORD_TO_THROW_Ecception;
	if (tinfo && tinfo->name())
	{
		typeinfo = tinfo->name();

		if (thrown_object)
		{
			std::exception* x = reinterpret_cast<std::exception*>(thrown_object); // this may crash
			msg += x->what();
		}
	}
	msg += " '" + typeinfo + "' ";
	std::cerr << msg << std::flush;

	std::exit(0);
}

void* __cxa_begin_catch(void* unwind_arg) throw()
{
	Logger::error("NYI: __cxa_begin_catch");
	return unwind_arg;
}

void __cxa_end_catch() throw()
{
	Logger::error("NYI: __cxa_end_catch");
}

#if defined (__cplusplus)
}
#endif