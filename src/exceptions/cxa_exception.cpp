#include <common/Log.h>

#include <exception>
#include <stdexcept>
#include <typeinfo>


// see: https://code.woboq.org/llvm/libcxxabi/src/cxa_exception.cpp.html

extern "C"
{

void *__cxa_allocate_exception(size_t thrown_size) throw()
{
	void* ptr = malloc(thrown_size);
	if(ptr == nullptr)
		std::terminate();
	//TODO: Need a logger to log error but not in the console
	return ptr;
}

void __cxa_throw(void *thrown_object, std::type_info *tinfo, void (*dest)(void *))
{
	std::string typeinfo =  "unknown";
	std::string msg =  "";
	if (tinfo && tinfo->name())
	{
		typeinfo = tinfo->name();

		if (thrown_object)
		{
			std::exception* x = reinterpret_cast<std::exception*>(thrown_object); // this may crash
			msg = x->what();
		}
	}
	msg = "Throwing exception. Exception's type info name: '" + typeinfo + "' message:'" + msg + "'";
	Logger::error(msg);

	std::terminate();
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

}
